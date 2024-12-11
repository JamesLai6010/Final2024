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

void OperationCenter::_update_monster() {
	std::vector<Monster*> &monsters = DataCenter::get_instance()->monsters;
	for(Monster *monster : monsters)
		monster->update();
}

void OperationCenter::_update_tower() {
	std::vector<Tower*> &towers = DataCenter::get_instance()->towers;
	for(Tower *tower : towers)
		tower->update();
}

void OperationCenter::_update_prop(){
	std::vector<Prop*> &props = DataCenter::get_instance()->props;
	for (Prop *p : props){
		p->update();
	}
}

void OperationCenter::_update_towerBullet() {
	std::vector<Bullet*> &towerBullets = DataCenter::get_instance()->towerBullets;
	for(Bullet *towerBullet : towerBullets)
		towerBullet->update();
	// Detect if a bullet flies too far (exceeds its fly distance limit), which means the bullet lifecycle has ended.
	for(size_t i = 0; i < towerBullets.size(); ++i) {
		if(towerBullets[i]->get_fly_dist() <= 0) {
			towerBullets.erase(towerBullets.begin()+i);
			--i;
		}
	}
}

void OperationCenter::_update_monster_towerBullet() {
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Monster*> &monsters = DC->monsters;
	std::vector<Bullet*> &towerBullets = DC->towerBullets;
	for(size_t i = 0; i < monsters.size(); ++i) {
		for(size_t j = 0; j < towerBullets.size(); ++j) {
			// Check if the bullet overlaps with the monster.
			if(monsters[i]->shape->overlap(*(towerBullets[j]->shape))) {
				// Reduce the HP of the monster. Delete the bullet.
				monsters[i]->HP -= towerBullets[j]->get_dmg();
				towerBullets.erase(towerBullets.begin()+j);
				--j;
			}
		}
	}
}

void OperationCenter::_update_monster_player() {
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Monster*> &monsters = DC->monsters;
	Player *&player = DC->player;
	for(size_t i = 0; i < monsters.size(); ++i) {
		// Check if the monster is killed.
		if(monsters[i]->HP <= 0) {
			// Monster gets killed. Player receives money.
			player->coin += monsters[i]->get_money();
			monsters.erase(monsters.begin()+i);
			--i;
			// Since the current monsster is killed, we can directly proceed to next monster.
			break;
		}
		// Check if the monster reaches the end.
		if(monsters[i]->get_path().empty()) {
			monsters.erase(monsters.begin()+i);
			player->HP--;
			--i;
		}
	}
}

//快龍
void OperationCenter::_update_monster_hero()
{
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Monster *> &monsters = DC->monsters;
	for (size_t i = 0; i < monsters.size(); ++i)
	{
		if (monsters[i]->shape->overlap(*(DC->hero->shape)))
		{
			monsters[i]->HP = 0;
		}
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
	_draw_monster();
	_draw_tower();
	_draw_towerBullet();
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

void OperationCenter::_draw_monster() {
	std::vector<Monster*> &monsters = DataCenter::get_instance()->monsters;
	for(Monster *monster : monsters)
		monster->draw();
}


void OperationCenter::_draw_tower() {
	std::vector<Tower*> &towers = DataCenter::get_instance()->towers;
	for(Tower *tower : towers)
		tower->draw();
}

void OperationCenter::_draw_towerBullet() {
	std::vector<Bullet*> &towerBullets = DataCenter::get_instance()->towerBullets;
	for(Bullet *towerBullet : towerBullets)
		towerBullet->draw();
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
	if (!(ch1_isAttack || ch2_isAttack)){return;}
	
	// 檢查方向使否對
	if (!((CH1._get_dir() == true && CH2._get_dir() == false)||(CH1._get_dir() == false && CH2._get_dir() == true))){
		if (CH1._get_dir() == true && CH2._get_dir() == true){
			if (ch1_isAttack && CH1.shape->center_x() < CH2.shape->center_x())ch1_isAttack = 0;
			if (ch2_isAttack && CH2.shape->center_x() < CH1.shape->center_x())ch2_isAttack = 0;
		}if (CH1._get_dir() == false && CH2._get_dir() == false){
			if (ch1_isAttack && CH1.shape->center_x() > CH2.shape->center_x())ch1_isAttack = 0;
			if (ch2_isAttack && CH2.shape->center_x() > CH1.shape->center_x())ch2_isAttack = 0;
		}
	}
	// 檢查距離對不對
	if ((CH1._get_dir() == true && CH2._get_dir() == false)){
		if (!(CH1.shape->center_x() >= CH2.shape->center_x()))return;
	}else{
		if (!(CH1.shape->center_x() <= CH2.shape->center_x()))return;
	}
	
	if (ch1_isAttack && ch2_isAttack){
		int x = (*DC->background_inf)._get_random_num();
		if (x%2){ // ch1_isAttack;
			CH2.set_state(CharacterState::HURT);
			CH1.attack_opponent(CH2);
		}else{
			CH1.set_state(CharacterState::HURT);
			CH2.attack_opponent(CH1);
		}
	}else if (ch1_isAttack){
		if (CH2._get_state() == CharacterState::SHIELD){
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
	}else if (ch2_isAttack){
		if (CH1._get_state() == CharacterState::SHIELD){
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
	if (ch1_isAttack) {
		std::cout << "Player 1 is attacking!" << std::endl;
		CharacterState current_state = CH1._get_state();
        if (current_state == CharacterState::ATTACK1) {
			if (player1_role == 1) {
				skill1_damage(CH1, CH2, 40); // 玩家2攻擊玩家1，扣40血
			} else if (player1_role == 2) {
				skill1_damage(CH1, CH2, 40); // 玩家2攻擊玩家1，扣40血
				skill1_poison(CH1, CH2, 1000);
			} else if (player2_role == 3) {
				skill1_damage(CH1, CH2, 40); // 玩家2攻擊玩家1，扣40血
			} else if (player2_role == 4) {
				skill1_damage(CH1, CH2, 40); // 玩家2攻擊玩家1，扣40血
			}
		} else if (current_state == CharacterState::ATTACK2) {
			if (player1_role == 1) {
				skill1_knockback(CH1, CH2, 200.0); // 距離 100，速度 10
			} else if (player1_role == 2) {
				skill1_knockback(CH1, CH2, 200.0); // 距離 100，速度 10
			} else if (player2_role == 3) {
				skill1_knockback(CH1, CH2, 200.0); // 距離 100，速度 10
			} else if (player2_role == 4) {
				skill1_knockback(CH1, CH2, 200.0); // 距離 100，速度 10
			}
		} else if (current_state == CharacterState::ATTACK3) {
			if (player1_role == 1) {
				skill1_knockback(CH1, CH2, 200.0); // 距離 100，速度 10
			} else if (player1_role == 2) {
				skill1_knockback(CH1, CH2, 200.0); // 距離 100，速度 10
			} else if (player2_role == 3) {
				skill1_knockback(CH1, CH2, 200.0); // 距離 100，速度 10
			} else if (player2_role == 4) {
				skill1_knockback(CH1, CH2, 200.0); // 距離 100，速度 10
			}
		}
    }

    if (ch2_isAttack) {
		std::cout << "Player 2 is attacking!" << std::endl;
    	CharacterState current_state = CH2._get_state();
    if (current_state == CharacterState::ATTACK1) {
        if (player2_role == 1) {
            skill1_damage(CH2, CH1, 35); // 玩家2角色1使用攻擊1
        } else if (player2_role == 2) {
            skill1_damage(CH2, CH1, 40); // 玩家2攻擊玩家1，扣40血
			skill1_poison(CH2, CH1, 1000);
        } else if (player2_role == 3) {
            skill1_damage(CH2, CH1, 45); // 玩家2角色3使用攻擊1
        } else if (player2_role == 4) {
            skill1_damage(CH2, CH1, 55); // 玩家2角色4使用攻擊1
        }
    } else if (current_state == CharacterState::ATTACK2) {
        if (player2_role == 1) {
            skill1_knockback(CH2, CH1, 200.0); // 距離 100，速度 10
        } else if (player2_role == 2) {
            skill1_knockback(CH2, CH1, 200.0); // 距離 100，速度 10
        } else if (player2_role == 3) {
            skill1_knockback(CH2, CH1, 200.0); // 距離 100，速度 10
        } else if (player2_role == 4) {
            skill1_knockback(CH2, CH1, 200.0); // 距離 100，速度 10
        }
    } else if (current_state == CharacterState::ATTACK3) {
        if (player2_role == 1) {
            skill1_knockback(CH2, CH1, 200.0); // 距離 100，速度 10
        } else if (player2_role == 2) {
            skill1_knockback(CH2, CH1, 200.0); // 距離 100，速度 10
        } else if (player2_role == 3) {
            skill1_knockback(CH2, CH1, 200.0); // 距離 100，速度 10
        } else if (player2_role == 4) {
            skill1_knockback(CH2, CH1, 200.0); // 距離 100，速度 10
        }
    }
	
	}
}


void OperationCenter::skill1_damage(CharacterBase& caster, CharacterBase& target, double damage) {
    target._set_HP(-damage - caster._get_ATKbias()); // 扣血
    std::cout << "Damage skill applied! Target HP: " << target._get_HP() << "\n";
}

void OperationCenter::skill1_knockback(CharacterBase& caster, CharacterBase& target, double distance) {
    double direction = (caster._get_dir() ? -1 : 1); // 根據方向決定滑行方向

    // 初始化滑行狀態
    target.set_state(CharacterState::SLIDE);         // 切換為受傷狀態
    target.start_knockback(distance, direction);   // 開始滑行（初始化距離和方向）
    target.set_slide_timer(1.0);                    // 設定受傷持續時間（例如 1 秒）

    std::cout << "Knockback started! Distance: " << distance << ", Direction: " << direction << std::endl;
}

void OperationCenter::skill1_poison(CharacterBase& caster, CharacterBase& target, double time){
	target._set_poisonTimer(time);
}

