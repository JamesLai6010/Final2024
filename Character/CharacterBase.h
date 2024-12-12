#ifndef CHARACTER_BASE_H_INCLUDED
#define CHARACTER_BASE_H_INCLUDED

#include <string>
#include <map>
#include "../Object.h"
#include "../algif5/algif.h"
#include <allegro5/allegro_primitives.h>
#include "../data/DataCenter.h"

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
    SLIDE,
    FREEZE,
    NONE
};

class DataCenter;

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
    void set_effect_val(double hp, double sp_t, double sp_b, double atk_t, double atk_b); //  Props effect
    // 選擇後重設gif
    void reset_gif_paths(const std::map<CharacterState, std::string>& new_gif_paths); // 重設 GIF 路徑

    // get character information
    double _get_HP() const;
    double _get_Rage() const;
    CharacterState _get_state();
    bool _get_dir();
    double _get_ATKtimer()const;

    // set character val
    double _set_HP(double hp);
    double _set_Rage(double rage);

    // attack oppent, set their val
    void attack_opponent(CharacterBase &opp);
    //擊退效果
    void start_knockback(double distance, double direction);
    void update_knockback();
    void set_slide_timer(double t);
    void _set_poisonTimer(double t);
    double _get_ATKbias();
    void _set_Rage_status(bool );
    void _set_Slowdown(bool, double);
    //freeze
    void _set_freeze(bool frozen, double duration); // 設置凍住狀態
    void update_freeze();                           // 更新凍住邏輯
    bool _is_frozen() const;                        // 返回是否凍住
    //護盾大招
    void _set_shield(double shield_value, double duration);
    void update_shield();
    bool _is_shielded() const { return is_shielded; }
    double _get_shield_value() const { return shield_value; }

    //角色移動相關
    void enforce_boundaries();   //邊界
    void handle_jump_logic(DataCenter* DC);  //jump
    void handle_attack_input(DataCenter* DC);  //attack
    void update_effects();   //效果

    //重新玩
    void reset();


protected:  


    // 狀態與動畫
    CharacterState state = CharacterState::NONE;
    ALGIF_ANIMATION* current_animation = nullptr;
    std::map<CharacterState, std::string> gifPath;

    ALGIF_ANIMATION* hp_effect_animation = nullptr;    // 血量效果動畫
    ALGIF_ANIMATION* speed_effect_animation = nullptr; // 速度效果動畫
    ALGIF_ANIMATION* atk_effect_animation = nullptr;   // 攻擊效果動畫
    ALGIF_ANIMATION* hit_animation = nullptr;
    ALGIF_ANIMATION* shield_animation = nullptr;

    // 角色參數
    double HP = 1000;
    double Rage = 0; // 怒氣值，觸發大招
    double speed = 5.0;
    double scale_x = 1.0, scale_y = 1.0;
    bool is_facing_left = false;
    bool is_attacking = false;
    bool is_jumping = false;
    bool is_hurting = false;
    bool is_poisoned = false;
    bool sliding = false;
    bool is_Rage_status = false;
    bool is_slow_down = false;

    bool is_frozen = false;        // 是否凍住
    double freeze_timer = 0.0;     // 凍住持續時間
    double slow_down_timer;

    //護盾
    double shield_value = 0;       // 護盾值
    double shield_timer = 0;       // 護盾持續時間
    bool is_shielded = false;      // 是否啟用護盾

    double slide_distance;
    double slide_direction;
    double slide_speed;
    double poison_timer;

    // 搭載的符文參數
    double shield_val = 0;
    double Rage_bias = 0;
    double ATK_percnt = 0;

    // 跳躍參數
    double gravity = 30.0;
    double vertical_velocity = 0.0;
    double jump_initial_velocity = 15.0;
    double ground_level = 600;

    // 動作計時
    double attack_timer = 0.0;
    double attack_duration = 0.5;
    double shield_duration = 0.7;
    double hurt_timer = 0.0;
    double slide_timer = 0.0;
    double hurt_duration = 0.4;

    // 效果計時器
    double Hp_timer = 0;
    double Speed_timer = 0;
    double speed_bias = 0; // 速度偏移值
    double Atk_timer = 0; 
    double Atk_bias = 0;

    // 初始位置
    double initial_x = 300;
    double initial_y = ground_level;

    // 按鍵配置
    int key_left, key_right, key_jump, key_attack1, key_attack2, key_attack3, key_shield;

    // 碰撞箱
    void update_bounding_box();

    

};

#endif
