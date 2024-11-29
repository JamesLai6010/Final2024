#ifndef MONSTERCAVEMAN_H_INCLUDED
#define MONSTERCAVEMAN_H_INCLUDED

#include "Monster.h"

// fixed settings: MonsterCaveMan attributes
class MonsterCaveMan : public Monster
{
public:
	MonsterCaveMan(const std::vector<Point> &path) : Monster{path, MonsterType::CAVEMAN} {
		HP = 25;
		v = 40;
		money = 20;
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3})); // UP
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3})); // DOWN
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3})); // LEFT
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3})); // RIGHT
		bitmap_switch_freq = 20;
	}
};

#endif
