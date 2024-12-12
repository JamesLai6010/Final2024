// Character2.cpp
#include "Character2.h"


void Character2::init() {
    
    gifPath[CharacterState::WALK] = "./assets/gif/Character2/Walk.gif";
    gifPath[CharacterState::ATTACK1] = "./assets/gif/Character2/Attack1.gif";
    gifPath[CharacterState::ATTACK2] = "./assets/gif/Character2/Attack2.gif";
    gifPath[CharacterState::ATTACK3] = "./assets/gif/Character2/Attack3.gif";
    gifPath[CharacterState::SHIELD] = "./assets/gif/Character2/Shield.gif";
    gifPath[CharacterState::RUN] = "./assets/gif/Character2/Run.gif";
    gifPath[CharacterState::STOP] = "./assets/gif/Character2/Stop.gif";
    gifPath[CharacterState::JUMP] = "./assets/gif/Character2/Jump.gif";
    gifPath[CharacterState::HURT] = "./assets/gif/Character2/Hurt.gif";
    gifPath[CharacterState::DEAD] = "./assets/gif/Character2/Dead.gif";
    gifPath[CharacterState::FREEZE] = "./assets/gif/Character2/Ice.gif";
    initial_x = 1300;  // 角色 2 的初始位置
    initial_y = ground_level;
    is_facing_left = true;
    CharacterBase::init();  // 調用基類的初始化

    set_key_mapping(ALLEGRO_KEY_LEFT,    // 左
                    ALLEGRO_KEY_RIGHT,   // 右
                    ALLEGRO_KEY_UP,      // 跳躍
                    ALLEGRO_KEY_M,       // 攻擊 1
                    ALLEGRO_KEY_COMMA,   // 攻擊 2
                    ALLEGRO_KEY_FULLSTOP, // 攻擊 3
                    ALLEGRO_KEY_SLASH    // 防禦
    );
}
