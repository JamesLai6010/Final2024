#ifndef CHARACTER1_H_INCLUDED
#define CHARACTER1_H_INCLUDED

#include <string>
#include <map>
#include "../Object.h"
#include "../algif5/algif.h"

enum class Character1State {
    WALK,      // 走路
    ATTACK1,   // 攻擊1
    ATTACK2,   // 攻擊2
    ATTACK3,   // 攻擊3
    NONE,       // 無動作
    STOP
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
    double speed = 5;                              // 角色移動速度
    int frame_index = 0;                           // 當前幀索引
    double frame_timer = 0.0;                      // 幀計時器
    double frame_duration = 0.1;                   // 每幀持續時間
    bool is_facing_left = false;                   // 角色是否面向左
    bool is_moving = false;
    //縮放圖片
    double scale_x = 1.0;                          // 水平縮放比例
    double scale_y = 1.0;                          // 垂直縮放比例
    void update_bounding_box(); // 根據縮放比例更新碰撞箱


    struct Animation {
        ALLEGRO_BITMAP* sprite_sheet; // 動畫的多幀圖片
        int frame_width;              // 單幀寬度
        int frame_height;             // 單幀高度
        int num_frames;               // 幀數
    };
    std::map<Character1State, Animation> animations; // 所有動畫
};

#endif
