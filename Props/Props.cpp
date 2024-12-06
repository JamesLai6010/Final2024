#include "Props.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../data/GIFCenter.h"
#include "../shapes/Circle.h"
#include "../shapes/Point.h"
#include <algorithm>
#include <allegro5/bitmap_draw.h>

/*
Prop::Prop(const Point &p, const Point &target, const std::string &path, double v, double val) {
	ImageCenter *IC = ImageCenter::get_instance();
	this->val = val;
	icon = IC->get(path);
    double r = std::min(al_get_bitmap_width(icon), al_get_bitmap_height(icon)) * 0.8;
	shape.reset(new Circle{p.x, p.y, r});
	double d = Point::dist(p, target);
}
*/

namespace PropSetting{
	static constexpr char prop_imgs_root_path[static_cast<int>(PropsType::PropsTpye_Max)][50] = {
		"./assets/image/Prop/2_Icons/Icon_05.png",
		"./assets/image/Prop/2_Icons/Icon_10.png",
		"./assets/image/Prop/2_Icons/Icon_06.png"

	};
	static constexpr char prop_spritesheet_root_path[static_cast<int>(PropsType::PropsTpye_Max)][40] = {
		"./assets/gif/Healthy_porp.gif",
		"./assets/gif/Haste_porp.gif",
		"./assets/gif/Crit_porp.gif"

	};
} // namespace PropSetting



Prop::Prop(const Point &p, const Point &target, PropsType Type ,double v, double val){ // path, val 自行處理
	ImageCenter *IC = ImageCenter::get_instance();
	this->icon = IC->get(PropSetting::prop_imgs_root_path[static_cast<int>(Type)]);
	GIFCenter *GIFC = GIFCenter::get_instance();
	magic.sprite_sheet = GIFC->get(PropSetting::prop_spritesheet_root_path[static_cast<int>(Type)]);
	
	double d = Point::dist(p, target);
	this->val = val;
	this->type = Type;
	this->vx = (target.x - p.x) * v / d;
	this->vy = (target.y - p.y) * v / d;
	this->fly_dist = Point::dist(p, target);

	double r = std::min(al_get_bitmap_width(icon), al_get_bitmap_height(icon)) * 0.8;
	shape.reset(new Circle{p.x, p.y, r});
}


Prop* Prop::Prop_Create(const Point &p, const Point &target, PropsType Type , double v, double val){
	return new Prop(p, target, Type, v, val);
}
/**
 * @brief Update the bullet position by its velocity and fly_dist by its movement per frame.
 * @details We don't detect whether to delete the bullet itself here because deleting a object itself doesn't make any sense.
 */


void Prop::update() {
	if(fly_dist == 0){
		return ;
	}
	DataCenter *DC = DataCenter::get_instance();
	double dx = vx / DC->FPS;
	double dy = vy / DC->FPS;
	double movement = Point::dist(Point{dx, dy}, Point{0, 0});
	if(fly_dist > movement) {
		shape->update_center_x(shape->center_x() + dx);
		shape->update_center_y(shape->center_y() + dy);
		fly_dist -= movement;
	} else {
		shape->update_center_x(shape->center_x() + dx * fly_dist / movement);
		shape->update_center_y(shape->center_y() + dy * fly_dist / movement);
		fly_dist = 0;
	}
	
}

void
Prop::draw() {
	if (fly_dist == -1)return ;
	al_draw_bitmap(
		icon,
		shape->center_x() - al_get_bitmap_width(icon) / 2,
		shape->center_y() - al_get_bitmap_height(icon) / 2, 0); // 左上角
}

void Prop::Prop_effect(Character1 &CH1){
	if (fly_dist == -1)return;

	switch (this->type)
	{
	case PropsType::Healthy:
		CH1.HP += 200; // wait
		break;
	case PropsType::Haste:
		
		
	
	default:
		break;

	algif_draw_gif(magic.sprite_sheet,
		shape->center_x() - magic.sprite_sheet->width / 2,
		shape->center_y() - magic.sprite_sheet->height / 2, 0);
	fly_dist = -1;
	}
	//delete this;
}