#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED
#include <string>
#include <map>
#include "Object.h"

enum class HeroState {
	LEFT, RIGHT, FRONT, BACK, HEROSTATE_MAX
};

class Hero : public Object
{
	public:
		void init();
		void update();
		void draw();
	private:
		HeroState state = HeroState::FRONT;
		double speed = 5;
		std::map<HeroState, std::string> gifPath;
};
#endif