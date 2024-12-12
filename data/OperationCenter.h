#ifndef OPERATIONCENTER_H_INCLUDED
#define OPERATIONCENTER_H_INCLUDED

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
	//大招護盾
	void skill_shield(CharacterBase& caster, double shield_value, double duration);
	
	void skill1(CharacterBase& caster, CharacterBase& target, int role_number);
	void skill2(CharacterBase& caster, CharacterBase& target, int role_number);
	void skill3(CharacterBase& caster, CharacterBase& target, int role_number);

	void set_player_roles(int player1_role, int player2_role);

private:
	OperationCenter() {}
private:
	void _update_monster();
	void _update_tower();
	void _update_towerBullet();
	void _update_prop();

	void _update_monster_towerBullet();
	void _update_monster_player();
	void _update_monster_hero();
	void _update_character_porp();
	void _update_character12();
	
private:
	void _draw_prop();
	void _draw_monster();
	void _draw_tower();
	void _draw_towerBullet();

	int player1_role = 0; // 玩家1角色編號
    int player2_role = 0; // 玩家2角色編號
};

#endif
