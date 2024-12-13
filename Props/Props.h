#ifndef PROP_H_INCLUDED
#define PROP_H_INCLUDED

#include "../Object.h"
#include <allegro5/bitmap.h>
#include <string>
#include "../Character/CharacterBase.h"


enum class PropsType{
	Healthy, Haste, Crit, PropsTpye_Max, // Haste: 加速, Crit: 爆擊、強化攻擊
};


class Prop : public Object
{
public:
	static Prop *Prop_Create(const Point &p, const Point &target, PropsType Type , double v, double val);
	void Prop_effect(CharacterBase &CH1);
	Prop(const Point &p, const Point &target, PropsType Type , double v, double val);
	void update();
	void draw();
	double get_fly_dict();
private:
	double const ground_base = 1500;
	double vx; // Velocity in x direction.
	double vy; // Velocity in y direction.
	double fly_dist;
	double val;
    /**
	 * @brief ALLEGRO_BITMAP of the bullet.
	 */
	ALLEGRO_BITMAP *icon;
    struct Animation {
        ALGIF_ANIMATION* sprite_sheet; // 動畫的多幀圖片
        int frame_width;              // 單幀寬度
        int frame_height;             // 單幀高度
        int num_frames;               // 幀數
    };
	PropsType type;
    ALGIF_ANIMATION* magic; // 所有動畫 精靈圖
	// time var.
};

#endif
