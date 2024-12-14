#ifndef OPERATIONCENTER_H_INCLUDED
#define OPERATIONCENTER_H_INCLUDED
#include "../Props/Meteor.h"
#include "SoundCenter.h"
#include <cstdlib> // For rand()
#include <ctime>   // For seeding rand()
/**
 * @brief Class that defines functions for all object operations.
 * @details Object self-update, draw, and object-to-object interact functions are defined here.
 */

class CharacterBase;
class OperationCenter
{
public:
	static OperationCenter *get_instance() {
		static OperationCenter OC;
		return &OC;
	}
	/**
	 * @brief Highest level update function.
	 * @details Calls all other update functions.
	 */
	void update();
	/**
	 * @brief Highest level draw function.
	 * @details Calls all other draw functions.
	 */
	void draw();

	// 技能函數
    void skill_damage(CharacterBase& caster, CharacterBase& target, double damage);
    void skill_knockback(CharacterBase& caster, CharacterBase& target, double distance);
	void skill_poison(CharacterBase& caster, CharacterBase& target, double time);
	void skill_SlowDown(CharacterBase& caster, CharacterBase& target, double time);
	void skill_freeze(CharacterBase& caster, CharacterBase& target, double duration);
	void skill_teleport_behind(CharacterBase& caster, CharacterBase& target, double distance);
	void skill_shoot(CharacterBase& caster, double time);
	//大招護盾
	void skill_shield(CharacterBase& caster, double shield_value, double duration);
	
	void skill1(CharacterBase& caster, CharacterBase& target, int role_number);
	void skill2(CharacterBase& caster, CharacterBase& target, int role_number);
	void skill3(CharacterBase& caster, CharacterBase& target, int role_number);
	
	void set_player_roles(int player1_role, int player2_role);

	void _update_projectiles();

	void spawn_meteor();  // 隕石生成
    void update_meteors(); // 更新隕石
    void draw_meteors();   // 繪製隕石
	void reset_meteors(); // 重置隕石
	// void _detect_far_attack();

	//void sprint_attack();
	//void skill_sprintATK(CharacterBase& caster, double dis, int state);


private:
	OperationCenter() {}
private:
	std::vector<Meteor> meteors; // 儲存所有隕石
    double meteorSpawnTimer = 0.0; // 用於計時生成隕石
    double meteorSpawnInterval = 2.0; // 隕石生成間隔（例如，每 2 秒生成一顆）
	/*
	void _update_monster();
	void _update_tower();
	void _update_towerBullet();

	void _update_monster_towerBullet();
	void _update_monster_player();
	void _update_monster_hero();

	void _draw_monster();
	void _draw_tower();
	void _draw_towerBullet();
	*/
	
	void _update_prop();

	
	void _update_character_porp();
	void _update_character12();
	
private:
	void _draw_prop();
	

	int player1_role = 0; // 玩家1角色編號
    int player2_role = 0; // 玩家2角色編號
};

#endif
