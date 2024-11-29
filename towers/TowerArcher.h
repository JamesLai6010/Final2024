#ifndef TOWERARCHER_H_INCLUDED
#define TOWERARCHER_H_INCLUDED

#include "Tower.h"
#include "Bullet.h"
#include "../shapes/Point.h"

// fixed settings: TowerArcher attributes
class TowerArcher : public Tower
{
public:
	TowerArcher(const Point &p) : Tower(p, attack_range(), 36, TowerType::ARCHER) {}
	Bullet *create_bullet(Object *target) {
		const Point &p = Point(shape->center_x(), shape->center_y());
		const Point &t = Point(target->shape->center_x(), target->shape->center_y());
		return new Bullet(p, t, TowerSetting::tower_bullet_img_path[static_cast<int>(type)], 480, 4, attack_range());
	}
	const double attack_range() const { return 160; }
};

#endif
