#ifndef CHARACTER_BASE_H_INCLUDED
#define CHARACTER_BASE_H_INCLUDED

#include <string>
#include <map>
#include "../Object.h"
#include "../algif5/algif.h"

// 通用角色狀態
enum class CharacterState {
    WALK,
    RUN,
    JUMP,
    ATTACK1,
    ATTACK2,
    ATTACK3,
    STOP,
    SHIELD,
    HURT,
    DEAD,
    NONE
};

// 角色基類
class CharacterBase : public Object {
public:
    // 公共方法
    virtual void init();
    virtual void update();
    virtual void draw();
    void set_state(CharacterState new_state);
    void set_scale(double sx, double sy);

    // 設定玩家按鍵配置
    void set_key_mapping(int left, int right, int jump, int attack1, int attack2, int attack3, int shield);

protected:
    // 狀態與動畫
    CharacterState state = CharacterState::NONE;
    ALGIF_ANIMATION* current_animation = nullptr;
    std::map<CharacterState, std::string> gifPath;

    // 角色參數
    double speed = 5.0;
    double scale_x = 1.0, scale_y = 1.0;
    bool is_facing_left = false;
    bool is_attacking = false;
    bool is_jumping = false;

    // 跳躍參數
    double gravity = 30.0;
    double vertical_velocity = 0.0;
    double jump_initial_velocity = 15.0;
    double ground_level = 600;

    // 動作計時
    double attack_timer = 0.0;
    double attack_duration = 0.5;
    double shield_duration = 0.7;

    // 初始位置
    double initial_x = 300;
    double initial_y = ground_level;

    // 按鍵配置
    int key_left, key_right, key_jump, key_attack1, key_attack2, key_attack3, key_shield;

    // 碰撞箱
    void update_bounding_box();
};

#endif