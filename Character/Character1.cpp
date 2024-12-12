#include "CharacterBase.h"
#include "Character1.h"
#include <map>


void Character1::init() {
    // 設定角色 1 的 GIF 路徑
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
