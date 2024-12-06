#include "DataCenter.h"
#include <cstring>
#include "../Level.h"
#include "../Player.h"
#include "../monsters/Monster.h"
#include "../towers/Tower.h"
#include "../towers/Bullet.h"
#include "../Hero.h"
#include "../Character/Character1.h"
#include "../Character/Character2.h"
#include "../Props/Props.h"
#include "../Props/Prop_GOD.h"

// fixed settings
namespace DataSetting {
	constexpr double FPS = 60;
	constexpr int window_width = 1600;
	constexpr int window_height = 900;
	constexpr int game_field_length = 600;
}

DataCenter::DataCenter() {
	this->FPS = DataSetting::FPS;
	this->window_width = DataSetting::window_width;
	this->window_height = DataSetting::window_height;
	this->game_field_length = DataSetting::game_field_length;
	memset(key_state, false, sizeof(key_state));
	memset(prev_key_state, false, sizeof(prev_key_state));
	mouse = Point(0, 0);
	memset(mouse_state, false, sizeof(mouse_state));
	memset(prev_mouse_state, false, sizeof(prev_mouse_state));
	player = new Player();
	level = new Level();
	//hero = new Hero();
	//新角色
	character1 = new Character1();
	//character1->init(); // 初始化角色
	character2 = new Character2();

	prop_god = new Prop_God(100);
	
}

DataCenter::~DataCenter() {
	delete player;
	delete level;
	delete character1;
	for(Monster *&m : monsters) {
		delete m;
	}
	for (Prop *&p: props){
		delete p;
	}
	for(Tower *&t : towers) {
		delete t;
	}
	for(Bullet *&b : towerBullets) {
		delete b;
	}
}
