#ifndef CHARACTER1_H_INCLUDED
#define CHARACTER1_H_INCLUDED

#include <string>
#include <map>
#include "../Object.h"
#include "../algif5/algif.h"

enum class Character1State {
    WALK,      // 走路
    RUN,
    JUMP,
    ATTACK1,   // 攻擊1
    ATTACK2,   // 攻擊2
    ATTACK3,   // 攻擊3
    STOP,      // 靜止
    SHIELD,    // 防禦
    DEAD,      // 死亡
    NONE,      // 無動作
};

class Character1 : public Object
{
public:
    void init();
    void update();
    void draw();
    void set_state(Character1State new_state);
    //放大縮小圖片
    void set_scale(double sx, double sy);
    
private:
    Character1State state = Character1State::NONE; // 當前狀態
    double speed = 5.0;   // 角色移動速度
    double scale_x = 1.0; // 水平縮放比例
    double scale_y = 1.0; // 垂直縮放比例
    bool is_facing_left = false; // 是否面向左側
    void update_bounding_box(); // 更新碰撞箱
    bool is_attacking = false;        // 是否處於攻擊狀態
    bool is_jumping = false;          // 是否處於跳躍狀態
    double attack_timer = 0.0;        // 攻擊動畫計時器
    double attack_duration = 0.5;     // 每次攻擊動畫的持續時間（秒）
    double shield_duration = 0.5;     // 防禦動畫的持續時間（秒）
    double jump_timer = 0.5;          // 跳躍計時器
    double jump_speed = 8.0;          // 跳躍速度

    double gravity = 30.0;       // 重力加速度（單位像素/秒²）
    double vertical_velocity = 0.0;  // 垂直速度（單位像素/秒）
    double jump_initial_velocity = 15.0; // 跳躍初速度（單位像素/秒）
    double ground_level = 600;        // 假設地面的位置（單位像素）

    std::map<Character1State, std::string> gifPath; // 各模式的 GIF 路徑
    ALGIF_ANIMATION* current_animation = nullptr;   // 當前的動畫
};

#endif
