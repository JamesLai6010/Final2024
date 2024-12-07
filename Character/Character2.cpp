// Character2.cpp
#include "Character2.h"


void Character2::init() {
    
    gifPath[CharacterState::WALK] = "Character2/Walk";
    gifPath[CharacterState::ATTACK1] = "Character2/Attack1";
    gifPath[CharacterState::ATTACK2] = "Character2/Attack2";
    gifPath[CharacterState::ATTACK3] = "Character2/Attack3";
    gifPath[CharacterState::SHIELD] = "Character2/Shield";
    gifPath[CharacterState::RUN] = "Character2/Run";
    gifPath[CharacterState::STOP] = "Character2/Stop";
    gifPath[CharacterState::JUMP] = "Character2/Jump";
    gifPath[CharacterState::HURT] = "Character2/Hurt";
    gifPath[CharacterState::DEAD] = "Character2/Dead";

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
