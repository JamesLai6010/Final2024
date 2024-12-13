#include "OperationCenter.h"
#include "DataCenter.h"
#include "../Background/GameInformation.h"
#include "../monsters/Monster.h"
#include "../towers/Tower.h"
#include "../towers/Bullet.h"
#include "../Player.h"
#include "../Hero.h"
#include "../Props/props.h"
#include "../Character/Character1.h"
#include "../Character/Character2.h"
#include "../Character/CharacterBase.h"
#include <iostream>

void OperationCenter::update() {
	// Update monsters.
	//_update_monster();
	// Update towers.
	//_update_tower();
	// Update tower bullets.
	//_update_towerBullet();
	// If any bullet overlaps with any monster, we delete the bullet, reduce the HP of the monster, and delete the monster if necessary.
	//_update_monster_towerBullet();
	// If any monster reaches the end, hurt the player and delete the monster.
	//_update_monster_player();
	//快龍
	//_update_monster_hero();

	_update_character12();

	_update_prop();
	_update_character_porp();

}


void OperationCenter::_update_prop(){
	std::vector<Prop*> &props = DataCenter::get_instance()->props;
	for (Prop *p : props){
		p->update();
	}
}


void OperationCenter::_update_character_porp(){
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Prop*> &props = DC->props;
	for (size_t i=0;i < props.size(); i++){
		if (props[i]->shape->overlap(*(DC->character1->shape))){
			props[i]->Prop_effect(*(DC->character1));
		}
		else if (props[i]->shape->overlap(*(DC->character2->shape))){
			props[i]->Prop_effect(*(DC->character2));
		}
	}
}

void OperationCenter::draw() {
	//_draw_monster();
	//_draw_tower();
	//_draw_towerBullet();
	_draw_prop();
}

void OperationCenter::_draw_prop(){
	std::vector<Prop*> &props = DataCenter::get_instance()->props;
	std::vector<Prop*> props_new;
	for (Prop *p : props){
		p->draw();
		if (p->get_fly_dict() > -20){
			props_new.emplace_back(p);
		}else{ // check this 
			//delete p;
		}
	}
	props = props_new;
}


void OperationCenter::set_player_roles(int player1_role, int player2_role) {
    this->player1_role = player1_role;
    this->player2_role = player2_role;
    std::cout << "Player 1 role set to: " << player1_role << std::endl;
    std::cout << "Player 2 role set to: " << player2_role << std::endl;
}


void OperationCenter::_update_character12() {
	
	DataCenter *DC = DataCenter::get_instance();
	CharacterBase &CH1 = *(DC->character1);
	CharacterBase &CH2 = *(DC->character2);

	// first check touch
	if (!(CH1.shape->overlap(*(DC->character2->shape)))){
		return;
	}
	bool ch1_isAttack = (CH1._get_state() == CharacterState::ATTACK1 || CH1._get_state() == CharacterState::ATTACK2 || CH1._get_state() == CharacterState::ATTACK3);
	bool ch2_isAttack = (CH2._get_state() == CharacterState::ATTACK1 || CH2._get_state() == CharacterState::ATTACK2 || CH2._get_state() == CharacterState::ATTACK3);

	if (!(ch1_isAttack || ch2_isAttack)) {return;} // 沒有角色攻擊，直接返回
	

	// 角色方向輸出（便於調試）
	//std::cout << "CH1 Facing " << (CH1._get_dir() ? "LEFT" : "RIGHT") << std::endl;
	//std::cout << "CH2 Facing " << (CH2._get_dir() ? "LEFT" : "RIGHT") << std::endl;

	// 攻擊方向和位置檢查
	bool ch1_valid = ch1_isAttack && 
		((CH1._get_dir() && CH1.shape->center_x() >= CH2.shape->center_x()) || // CH1 面向左，且 CH2 在左
        (!CH1._get_dir() && CH1.shape->center_x() < CH2.shape->center_x()));  // CH1 面向右，且 CH2 在右邊

	bool ch2_valid = ch2_isAttack && 
		((CH2._get_dir() && CH2.shape->center_x() >= CH1.shape->center_x()) || // CH2 面向左，且 CH1 在左
        (!CH2._get_dir() && CH2.shape->center_x() < CH1.shape->center_x()));  // CH2 面向右，且 CH1 在右邊

	//if (ch1_valid) std::cout << "CH1 attack CH2!" << std::endl;
	//if (ch2_valid) std::cout << "CH2 attack CH1!" << std::endl;
    
	// 處理同時攻擊的狀況
	if (ch2_valid && ch1_valid){
		int x = (*DC->background_inf)._get_random_num();
		if (x%2){ // ch1_isAttack;
			ch2_valid = false;
		}else{
			ch1_valid = false;
		}
	}
	if (ch1_valid){
		if (CH2._get_state() == CharacterState::SHIELD){
			if (CH1._get_ATKtimer() - 0.5 != 0)return;
			CH2._set_Rage(20);
			return;
		}
		//std::cout << "CH1 timer: "<< CH1._get_ATKtimer() << std::endl;
		if (CH2._get_state() == CharacterState::HURT || CH1._get_ATKtimer() - 0.5 != 0){
			//std::cout << "HIT1" << std::endl;
			return;
		}else{
			CH1.attack_opponent(CH2);
			CH2.set_state(CharacterState::HURT);
		}
	}else if (ch2_valid){
		if (CH1._get_state() == CharacterState::SHIELD){
			if (CH2._get_ATKtimer() - 0.5 != 0)return;
			CH1._set_Rage(20);
			return;
		}
		//std::cout << "CH2 timer: "<< CH2._get_ATKtimer() << std::endl;
		if (CH1._get_state() == CharacterState::HURT || CH2._get_ATKtimer() - 0.5 != 0){
			//std::cout << "HIT2" << std::endl;
			return;
		}else{
			CH2.attack_opponent(CH1);
			CH1.set_state(CharacterState::HURT);
		}
	}
	if (ch1_valid) {
		//std::cout << "Player 1 is attacking!" << std::endl;
		CharacterState current_state = CH1._get_state();
        if (current_state == CharacterState::ATTACK1) {
			skill1(CH1, CH2, player1_role);
		} else if (current_state == CharacterState::ATTACK2) {
			skill2(CH1, CH2, player1_role);
		} else if (current_state == CharacterState::ATTACK3) {
			skill3(CH1, CH2, player1_role);
		}
    }

    if (ch2_valid) {
		//std::cout << "Player 2 is attacking!" << std::endl;
    	CharacterState current_state = CH2._get_state();
		if (current_state == CharacterState::ATTACK1) {
			skill1(CH2, CH1, player2_role);
		} else if (current_state == CharacterState::ATTACK2) {
			skill2(CH2, CH1, player2_role);
		} else if (current_state == CharacterState::ATTACK3) {
			skill3(CH2, CH1, player2_role);
		}
	
	}
}




void OperationCenter::skill_damage(CharacterBase& caster, CharacterBase& target, double damage) {
    target._set_HP(-(damage + caster._get_ATKbias())); // 扣血
    std::cout << "Damage skill applied! Target HP: " << target._get_HP() << "\n";
}

void OperationCenter::skill_knockback(CharacterBase& caster, CharacterBase& target, double distance) {
    double direction = (caster._get_dir() ? -1 : 1); // 根據方向決定滑行方向

    // 初始化滑行狀態
    target.set_state(CharacterState::SLIDE);         // 切換為受傷狀態
    target.start_knockback(distance, direction);   // 開始滑行（初始化距離和方向）
    target.set_slide_timer(1.0);                    // 設定受傷持續時間（例如 1 秒）

    std::cout << "Knockback started! Distance: " << distance << ", Direction: " << direction << std::endl;
}

void OperationCenter::skill_sprint(CharacterBase& caster, CharacterBase& target, double distance, int atk_type){
	double dir = (caster._get_dir())? -1:1;
	if (atk_type == 1)
		caster.start_sprint(distance, dir, CharacterState::ATTACK1);
}	

void OperationCenter::skill_poison(CharacterBase& caster, CharacterBase& target, double time){
	target._set_poisonTimer(time);
}

void OperationCenter::skill_SlowDown(CharacterBase& caster, CharacterBase& target, double time){
	target._set_Slowdown(true, time);
}

void OperationCenter::skill1(CharacterBase& caster, CharacterBase& target, int role_number){
	if (role_number == 1) {
		skill_sprint(caster, target, 500.0, 1);
		//skill_freeze(caster, target, 2.0); // 凍住 2 秒
		//skill_damage(caster, target, 40);
	} else if (role_number == 2) {
		skill_freeze(caster, target, 2.0); // 凍住 2 秒
		skill_damage(caster, target, 40); // 玩家2攻擊玩家1，扣40血
		
	} else if (role_number == 3) {
		skill_freeze(caster, target, 2.0); // 凍住 2 秒
		skill_damage(caster, target, 45); // 玩家2角色3使用攻擊1
	} else if (role_number == 4) {
		skill_freeze(caster, target, 2.0); // 凍住 2 秒
		skill_damage(caster, target, 55); // 玩家2角色4使用攻擊1
	}
}

void OperationCenter::skill2(CharacterBase& caster, CharacterBase& target, int role_number){
	if (role_number == 1) {
		skill_teleport_behind(caster, target, 200.0); // 傳送距離設為 50 像素
		//skill_knockback(caster, target, 200.0); // 距離 100，速度 10
	} else if (role_number == 2) {
		//skill_damage(caster, target, 35);
		skill_knockback(caster, target, 200.0); // 距離 100，速度 10
		skill_poison(caster, target, 10);
	} else if (role_number == 3) {
		skill_knockback(caster, target, 200.0); // 距離 100，速度 10
	} else if (role_number == 4) {
		skill_knockback(caster, target, 200.0); // 距離 100，速度 10
	}
}

void OperationCenter::skill3(CharacterBase& caster, CharacterBase& target, int role_number){
	if (caster._get_Rage() >= 100){
		
		if (role_number == 1) {
			skill_shield(caster, 200, 5.0); // 為自己提供150護盾值，持續5秒
			skill_SlowDown(caster, target, 3);
		} else if (role_number == 2) {
			skill_shield(caster, 200, 5.0); // 為自己提供150護盾值，持續5秒
			skill_SlowDown(caster, target, 3);
		} else if (role_number == 3) {
			skill_shield(caster, 200, 5.0); // 為自己提供150護盾值，持續5秒
			skill_SlowDown(caster, target, 3);
		} else if (role_number == 4) {
			skill_shield(caster, 200, 5.0); // 為自己提供150護盾值，持續5秒
			skill_SlowDown(caster, target, 3);
		}
		caster._set_Rage(-1*caster._get_Rage());
		return;
	}

	if (role_number == 1) {
		skill_knockback(caster, target, 200.0); // 距離 100，速度 10
	} else if (role_number == 2) {
		skill_knockback(caster, target, 200.0); // 距離 100，速度 10
	} else if (role_number == 3) {
		skill_knockback(caster, target, 200.0); // 距離 100，速度 10
	} else if (role_number == 4) {
		skill_knockback(caster, target, 200.0); // 距離 100，速度 10
	}
}

//冰凍
void OperationCenter::skill_freeze(CharacterBase& caster, CharacterBase& target, double duration) {
    target._set_freeze(true, duration);  // 設置目標為凍住狀態並設定持續時間
    std::cout << "Freeze skill applied! Target is frozen for " << duration << " seconds.\n";
}


void OperationCenter::skill_shield(CharacterBase& caster, double shield_value, double duration) {
    caster._set_shield(shield_value, duration); // 為角色設置護盾值與持續時間
    std::cout << "Shield skill activated! Value: " << shield_value << ", Duration: " << duration << " seconds.\n";
}

void OperationCenter::skill_teleport_behind(CharacterBase& caster, CharacterBase& target, double distance) {
    // 獲取敵人當前位置和面向方向
    double target_x = target.shape->center_x();
    double target_y = target.shape->center_y();
    bool target_facing_left = target._get_dir(); // true: 面向左, false: 面向右

    // 計算背後的位置
    double new_x = target_x + (target_facing_left ? distance : -distance);
    double new_y = target_y; // 假設同一高度
	caster._set_tp_timer(0.5);
    // 更新角色位置
    caster.shape->update_center_x(new_x);
    caster.shape->update_center_y(new_y);

    // 確保不超出邊界 (可選)
    caster.enforce_boundaries();

    // 輸出調試信息
    std::cout << "Teleport behind activated! New Position: (" 
              << new_x << ", " << new_y << ")" << std::endl;
}



