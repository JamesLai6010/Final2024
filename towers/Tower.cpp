#include "Tower.h"
#include "TowerArcane.h"
#include "TowerArcher.h"
#include "TowerCanon.h"
#include "TowerPoison.h"
#include "TowerStorm.h"
#include "../Utils.h"
#include "../shapes/Circle.h"
#include "../monsters/Monster.h"
#include "../shapes/Rectangle.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../data/SoundCenter.h"
#include <allegro5/bitmap_draw.h>

// fixed settings
namespace TowerSetting {
	constexpr char attack_sound_path[] = "./assets/sound/Arrow.wav";
};

ALLEGRO_BITMAP*
Tower::get_bitmap(TowerType type) {
	ImageCenter *IC = ImageCenter::get_instance();
	return IC->get(TowerSetting::tower_full_img_path[static_cast<int>(type)]);
}

Tower*
Tower::create_tower(TowerType type, const Point &p) {
	switch(type) {
		case TowerType::ARCANE: {
			return new TowerArcane(p);
		} case TowerType::ARCHER: {
			return new TowerArcher(p);
		} case TowerType::CANON: {
			return new TowerCanon(p);
		} case TowerType::POISON: {
			return new TowerPoison(p);
		} case TowerType::STORM: {
			return new TowerStorm(p);
		} case TowerType::TOWERTYPE_MAX: {}
	}
	GAME_ASSERT(false, "tower type error.");
}

/**
 * @param p center point (x, y).
 * @param attack_range any monster inside this number would trigger attack.
 * @param attack_freq period for tower to attack.
 * @param type tower type.
*/
Tower::Tower(const Point &p, double attack_range, int attack_freq, TowerType type) {
	ImageCenter *IC = ImageCenter::get_instance();
	// shape here is used to represent the tower's defending region. If any monster walks into this area (i.e. the bounding box of the monster and defending region of the tower has overlap), the tower should attack.
	shape.reset(new Circle(p.x, p.y, attack_range));
	counter = 0;
	this->attack_freq = attack_freq;
	this->type = type;
	bitmap = IC->get(TowerSetting::tower_full_img_path[static_cast<int>(type)]);
}

/**
 * @brief Update attack cooldown and detect if the tower could make an attack.
 * @see Tower::attack(Object *target)
*/
void
Tower::update() {
	if(counter) counter--;
	else {
		DataCenter *DC = DataCenter::get_instance();
		for(Monster *monster : DC->monsters) {
			if(attack(monster)) break;
		}
	}
}

/**
 * @brief Check whether the tower can attack the target. If so, shoot a bullet to the target.
*/
bool
Tower::attack(Object *target) {
	if(counter) return false;
	if(!target->shape->overlap(*shape)) return false;
	DataCenter *DC = DataCenter::get_instance();
	SoundCenter *SC = SoundCenter::get_instance();
	DC->towerBullets.emplace_back(create_bullet(target));
	SC->play(TowerSetting::attack_sound_path, ALLEGRO_PLAYMODE_ONCE);
	counter = attack_freq;
	return true;
}

void
Tower::draw() {
	al_draw_bitmap(
		bitmap,
		shape->center_x() - al_get_bitmap_width(bitmap)/2,
		shape->center_y() - al_get_bitmap_height(bitmap)/2, 0);
}

/**
 * @brief Get the area of the tower, and return with a Rectangle object.
*/
Rectangle
Tower::get_region() const {
	int w = al_get_bitmap_width(bitmap);
	int h = al_get_bitmap_height(bitmap);
	return {
		shape->center_x() - w/2,
		shape->center_y() - h/2,
		shape->center_x() - w/2 + w,
		shape->center_y() - h/2 + h
	};
}
