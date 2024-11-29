#ifndef MONSTERWOLFKNIGHT_H_INCLUDED
#define MONSTERWOLFKNIGHT_H_INCLUDED

#include "Monster.h"

// fixed settings: MonsterWolfKnight attributes
class MonsterWolfKnight : public Monster
{
public:
	MonsterWolfKnight(const std::vector<Point> &path) : Monster{path, MonsterType::WOLFKNIGHT} {
		HP = 15;
		v = 80;
		money = 30;
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3})); // UP
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3})); // DOWN
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3})); // LEFT
		bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3})); // RIGHT
		bitmap_switch_freq = 20;
	}
};

#endif
