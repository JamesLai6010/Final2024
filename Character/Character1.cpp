#include "CharacterBase.h"
#include "Character1.h"
#include <map>


void Character1::init() {
    // 設定角色 1 的 GIF 路徑
    gifPath[CharacterState::WALK] = "Character1/Walk";
    gifPath[CharacterState::RUN] = "Character1/Run";
    gifPath[CharacterState::JUMP] = "Character1/Jump";
    gifPath[CharacterState::ATTACK1] = "Character1/Attack1";
    gifPath[CharacterState::ATTACK2] = "Character1/Attack2";
    gifPath[CharacterState::ATTACK3] = "Character1/Attack3";
    gifPath[CharacterState::STOP] = "Character1/Stop";
    gifPath[CharacterState::SHIELD] = "Character1/Shield";
    gifPath[CharacterState::DEAD] = "Character1/Dead";

    // 初始位置
    initial_x = 300;
    initial_y = ground_level;

    // 調用基類初始化
    CharacterBase::init();

    // 設定按鍵配置（玩家 1 的按鍵）
    set_key_mapping(ALLEGRO_KEY_A,  // 左
                    ALLEGRO_KEY_D,  // 右
                    ALLEGRO_KEY_W,  // 跳躍
                    ALLEGRO_KEY_F,  // 攻擊 1
                    ALLEGRO_KEY_G,  // 攻擊 2
                    ALLEGRO_KEY_H,  // 攻擊 3
                    ALLEGRO_KEY_J   // 防禦
    );
}
