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
    void _set_HP(double hp);
    double _set_Rage(double rage);

    // attack oppent, set their val
    void attack_opponent(CharacterBase &opp); // no use
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
    //tp gif
    void _set_tp_timer(double t);
    //角色移動相關
    void enforce_boundaries();   //邊界
    void handle_jump_logic(DataCenter* DC);  //jump
    void handle_attack_input(DataCenter* DC);  //attack
    void update_effects();   //效果

    void shoot(double time);                  // 發射射出物
    void update_projectiles();     // 更新射出物位置
    void draw_projectiles();       // 繪製射出物


    void sprint_init(int dir, double distance, CharacterState nxt_state);
    void sprint_update();
    double _get_sprint_timer();
    void _set_sprint_flag(bool);
    bool _get_sprint_flag();
    
    void _set_role_number(int x){
        role = x;
    }
    //重新玩
    void reset();

    void start_invincible(double t);
    void invincible_update();

    void lifesteal(double t);
    bool _get_lifesteal();
    struct Projectile {
        double x;          // 當前的 x 座標
        double y;          // 當前的 y 座標
        double velocity_x; // 水平速度
        double velocity_y; // 垂直速度（若不需要垂直移動，可設為0）
        double lifetime;   // 存活時間
        std::unique_ptr<Shape> shape; // 碰撞箱
    };
    std::vector<Projectile> projectiles; // 儲存射出物的容器


protected:  


    bool is_invincible = false;
    double invincible_time = 0;
    double invincible_cd = 0;

    bool ATK_flag = false;
    CharacterState atk_flag_state = CharacterState::NONE;
    bool is_sprint = false;
    double sprint_distance;
    double sprint_dir;
    double sprint_speed = 0.0;
    double sprint_timer = 0;
    bool sprint_dective = false;


    // 狀態與動畫
    CharacterState state = CharacterState::NONE;
    ALGIF_ANIMATION* current_animation = nullptr;
    std::map<CharacterState, std::string> gifPath;
    ALGIF_ANIMATION* invincible_animation = nullptr;
    ALGIF_ANIMATION* hp_effect_animation = nullptr;    // 血量效果動畫
    ALGIF_ANIMATION* speed_effect_animation = nullptr; // 速度效果動畫
    ALGIF_ANIMATION* atk_effect_animation = nullptr;   // 攻擊效果動畫
    ALGIF_ANIMATION* hit_animation = nullptr;
    ALGIF_ANIMATION* shield_animation = nullptr;
    ALGIF_ANIMATION* poison_animation = nullptr;
    ALGIF_ANIMATION* teleport_animation = nullptr;
    ALGIF_ANIMATION* bulletLEFT_animation = nullptr;
    ALGIF_ANIMATION* bulletRIGHT_animation = nullptr;
    ALGIF_ANIMATION* lifesteal_animation = nullptr;
    ALGIF_ANIMATION* smoke_animation = nullptr;
    // 角色參數
    double HP = 1000;
    double max_HP = 1000;
    double Rage = 0; // 怒氣值，觸發大招
    double speed = 8.0;
    double scale_x = 1.0, scale_y = 1.0;
    bool is_facing_left = false;
    bool is_attacking = false;
    bool is_jumping = false;
    bool is_hurting = false;
    bool is_poisoned = false;
    bool sliding = false;
    bool is_Rage_status = false;
    bool is_slow_down = false;
    bool tp_gif = false;

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
    double tp_gif_timer = 0.0;
    double life_steal_timer = 0.0;
    // 初始位置
    double initial_x = 300;
    double initial_y = ground_level;

    // 按鍵配置
    int key_left, key_right, key_jump, key_attack1, key_attack2, key_attack3, key_shield;

    // 碰撞箱
    void update_bounding_box();

    int role = 0;

    

};

#endif
