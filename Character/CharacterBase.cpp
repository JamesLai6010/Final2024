#include "CharacterBase.h"
#include "../data/DataCenter.h"
#include "../algif5/algif.h"
#include <stdexcept>
#include "../shapes/Rectangle.h"
#include "../data/GIFCenter.h"
#include "../data/SoundCenter.h"
#include <iostream>

constexpr char minecraftHit_sound_path[] = "./assets/sound/MinecraftHit.mp3";

void CharacterBase::update_bounding_box() {
    if (current_animation) {
        float center_x = shape->center_x();
        float center_y = shape->center_y();
        float new_width = current_animation->width * scale_x;
        float new_height = current_animation->height * scale_y;

        shape.reset(new Rectangle(
            center_x - new_width / 2,
            center_y - new_height / 2,
            center_x + new_width / 2,
            center_y + new_height / 2
        ));
    }
}

void CharacterBase::set_scale(double sx, double sy) {
    scale_x = sx;
    scale_y = sy;
    update_bounding_box();
}

void CharacterBase::init() {
    GIFCenter* GIFC = GIFCenter::get_instance();
    DataCenter* DC = DataCenter::get_instance();
    SoundCenter *SC = SoundCenter::get_instance();

    // 加載狀態動畫
    hp_effect_animation = GIFC->get("./assets/gif/minecraft_effect4.gif");
    speed_effect_animation = GIFC->get("./assets/gif/Speed_effect.gif");
    atk_effect_animation = GIFC->get("./assets/gif/minecraft_effect2.gif");
    hit_animation = GIFC->get("./assets/gif/Hit.gif");
    shield_animation = GIFC->get("./assets/gif/Shield.gif");
    poison_animation = GIFC->get("./assets/gif/Poison.gif");
    teleport_animation = GIFC->get("./assets/gif/tp.gif");
    bulletLEFT_animation = GIFC->get("./assets/gif/shootLEFT.gif");
    //bulletRIGHT_animation = GIFC->get("./assets/gif/shootRIGHT.gif");

    // 加載初始動畫 (靜止)
    current_animation = GIFC->get(gifPath[CharacterState::STOP]);
    shape.reset(new Rectangle(
        initial_x - current_animation->width / 2,
        initial_y - current_animation->height / 2,
        initial_x + current_animation->width / 2,
        initial_y + current_animation->height / 2
    ));
    update_bounding_box();
}

void CharacterBase::set_state(CharacterState new_state) {
    SoundCenter *SC = SoundCenter::get_instance();
    if (state != new_state) {
        state = new_state;
        if (state == CharacterState::HURT) {
            SC->play(minecraftHit_sound_path, ALLEGRO_PLAYMODE_ONCE);
            hurt_timer = hurt_duration;
            is_hurting = true;
        }
        GIFCenter* GIFC = GIFCenter::get_instance();
        //slide用hurt gif
        if (state == CharacterState::SLIDE) {
            current_animation = GIFC->get(gifPath[CharacterState::HURT]);
        } else current_animation = GIFC->get(gifPath[state]);
        // for debug
        std::string s;
            if (state == CharacterState::WALK) s = "WALK";
            else if (state == CharacterState::RUN) s = "RUN";
            else if (state == CharacterState::JUMP) s = "JUMP";
            else if (state == CharacterState::ATTACK1) s = "ATTACK1";
            else if (state == CharacterState::ATTACK2) s = "ATTACK2";
            else if (state == CharacterState::ATTACK3) s = "ATTACK3";
            else if (state == CharacterState::STOP) s = "STOP";
            else if (state == CharacterState::SHIELD) s = "SHIELD";
            else if (state == CharacterState::HURT) s = "HURT";
            else if (state == CharacterState::DEAD) s = "DEAD";
            else if (state == CharacterState::SLIDE) s = "SLIDE";
            else if (state == CharacterState::FREEZE) s = "FREEZE";
            else if (state == CharacterState::NONE) s = "NONE";
            else s = "UNKNOWN";
        std::cout << "<Player State> : set to " << s << std::endl;
        update_bounding_box();
    }
}

void CharacterBase::set_key_mapping(int left, int right, int jump, int attack1, int attack2, int attack3, int shield) {
    key_left = left;
    key_right = right;
    key_jump = jump;
    key_attack1 = attack1;
    key_attack2 = attack2;
    key_attack3 = attack3;
    key_shield = shield;
}

void CharacterBase::update() {
    DataCenter* DC = DataCenter::get_instance();
    Rage = std::min(Rage, 100.0); // 限制 Rage 最大值
    // 更新凍住狀態
    update_freeze();
    update_shield();
    update_effects();
    update_projectiles();
    // 如果角色被凍住，僅阻止行為更新，但允許效果計時器繼續更新
    if (is_frozen) {
         // 更新狀態效果計時器
        return;
    }

    // 更新效果計時器

    // 處理移動與行為邏輯
    double now_speed = (is_slow_down) ? 2 : (speed + speed_bias); // 緩速
    if (state != CharacterState::SHIELD) {
        // 水平移動
        if (DC->key_state[key_right]) {
            shape->update_center_x(shape->center_x() + now_speed);
            is_facing_left = false; // 面向右
            if (!is_attacking && !is_jumping) {
                set_state(CharacterState::WALK);
            }
        } else if (DC->key_state[key_left]) {
            shape->update_center_x(shape->center_x() - now_speed);
            is_facing_left = true; // 面向左
            if (!is_attacking && !is_jumping) {
                set_state(CharacterState::WALK);
            }
        } else if (!is_attacking && !is_jumping && !is_hurting && !is_sprint) {
            set_state(CharacterState::STOP); // 停止狀態
        }
    }

    // 攻擊邏輯
    if (is_attacking) {
        attack_timer -= 1.0 / 60.0;
        if (attack_timer <= 0) {
            is_attacking = false;
            set_state(CharacterState::STOP); // 返回停止狀態
        }
    } else {
         // 處理攻擊輸入
        handle_attack_input(DC);
    }
    
    if (!is_attacking){
        handle_attack_input(DC);
    }

    // 處理跳躍邏輯
    handle_jump_logic(DC);

    // 更新滑行
    if (sliding) {
        update_knockback();
        slide_timer -= 1.0 / 60.0;
        if (slide_timer <= 0) {
            sliding = false;
            set_state(CharacterState::STOP);
        }
    }

    if (is_sprint){
        sprint_update();
        sprint_timer -= 1.0 / 60.0;
    }

    // 邊界檢查
    enforce_boundaries();
}

//各項效果，輸入統整為秒
void CharacterBase::update_effects() {
    // 更新速度加成效果
    if (Speed_timer > 0) { 
        Speed_timer -= 1.0 / 60.0;
    } else {
        Speed_timer = 0;
        speed_bias = 0;
    }

    // 更新攻擊加成效果
    if (Atk_timer > 0) {   //輸入是秒
        Atk_timer -= 1.0 / 60.0;
    } else {
        Atk_timer = 0;
        Atk_bias = 0;
    }

    // 更新生命回復效果
    if (Hp_timer > 0) {   //輸入是秒
        Hp_timer -= 1.0 / 60.0;
    } else {
        Hp_timer = 0;
    }

    // 更新緩速效果
    if (is_slow_down) {   //輸入是秒
        slow_down_timer -= 1.0 / 60.0;
        if (slow_down_timer <= 0) {
            is_slow_down = false;
        }
    }

    // 更新中毒效果
    if (is_poisoned) {   
        poison_timer -= 1.0 / 60.0;
        if (poison_timer <= 0) {
            is_poisoned = false;
        }
        if ((int)poison_timer % 2) {
            HP -= 0.1; // 每秒扣血
        }
    }

    // 更新受傷效果
    if (is_hurting) {     //輸入是秒
        hurt_timer -= 1.0 / 60.0;
        if (hurt_timer <= 0) {
            is_hurting = false; // 受傷狀態結束
            //set_state(CharacterState::STOP); // 回到停止狀態
        }
    }

    if (tp_gif) {      //輸入是秒
        tp_gif_timer -= 1.0 / 60.0;
        if (tp_gif_timer <= 0) {
            tp_gif = false;
        }
    }
}


//處理攻擊
void CharacterBase::handle_attack_input(DataCenter* DC) {
    if (DC->key_state[key_attack1]) {
        if (role == 1){
            int d = (is_facing_left)? -1:1;
            sprint_init(d, 400.0, CharacterState::ATTACK1);
        }else{
            set_state(CharacterState::ATTACK1);
            is_attacking = true;
            attack_timer = attack_duration;
        }
        
        Rage += 5;
    } else if (DC->key_state[key_attack2]) {
        set_state(CharacterState::ATTACK2);
        is_attacking = true;
        attack_timer = attack_duration;
        Rage += 5;
    } else if (DC->key_state[key_attack3]) {
        set_state(CharacterState::ATTACK3);
        is_attacking = true;
        attack_timer = attack_duration;
        Rage += 5;
    } else if (DC->key_state[key_shield]) {
        set_state(CharacterState::SHIELD);
        is_attacking = true;
        attack_timer = shield_duration;
    }
}
//處理跳躍
void CharacterBase::handle_jump_logic(DataCenter* DC) {
    if (is_jumping) {
        // 更新垂直速度（加入重力影響）
        vertical_velocity += gravity * (1.0 / 60.0);
        shape->update_center_y(shape->center_y() + vertical_velocity);

        // 檢查是否到達地面
        if (shape->center_y() >= ground_level) {
            shape->update_center_y(ground_level);
            is_jumping = false;
            vertical_velocity = 0;
            set_state(CharacterState::STOP);
        }
    }if (!is_jumping && DC->key_state[key_jump]) {
        is_jumping = true;
        vertical_velocity = -jump_initial_velocity;
        set_state(CharacterState::JUMP);
    }
}

//處理邊界
void CharacterBase::enforce_boundaries() {
    float current_x = shape->center_x();
    if (current_x < 0) {
        shape->update_center_x(0);
    } else if (current_x > 1600) {
        shape->update_center_x(1600);
    }
}

void CharacterBase::draw() {
    if (!current_animation) return;

    float draw_x = shape->center_x() - (current_animation->width * scale_x) / 2;
    float draw_y = shape->center_y() - (current_animation->height * scale_y) / 2;

    int flags = is_facing_left ? ALLEGRO_FLIP_HORIZONTAL : 0;

    algif_draw_gif(current_animation, draw_x, draw_y, flags);

    //畫bound_box
    al_draw_rectangle(draw_x, draw_y, draw_x+current_animation->width, draw_y+current_animation->height, al_map_rgb(255,255,255), 3);
    // 藥水效果動畫
    if (Speed_timer > 0 && speed_effect_animation) {
        float effect_x = shape->center_x() - (speed_effect_animation->width * scale_x) / 2;
        float effect_y = shape->center_y() + (current_animation->height * scale_y) / 2 -(speed_effect_animation->height * scale_y);
        algif_draw_gif(speed_effect_animation, effect_x, effect_y, 0);
    }

    if (Atk_timer > 0 && atk_effect_animation) {
        float effect_x = shape->center_x() - (atk_effect_animation->width * scale_x) / 2;
        float effect_y = shape->center_y() - (atk_effect_animation->height * scale_y) / 2;
        //printf("Atk_timer: %lf", Atk_timer);
        algif_draw_gif(atk_effect_animation, effect_x, effect_y, 0);
    }

    if (Hp_timer > 0 && hp_effect_animation) {
        float effect_x = shape->center_x() - (hp_effect_animation->width * scale_x) / 2;
        float effect_y = shape->center_y() - (hp_effect_animation->height * scale_y) / 2;
        algif_draw_gif(hp_effect_animation, effect_x, effect_y, 0);
    }

    if (is_hurting) {
        float effect_x = shape->center_x() - (hit_animation->width * scale_x) / 2;
        float effect_y = shape->center_y() - (hit_animation->height * scale_y) / 2 + 30;
        algif_draw_gif(hit_animation, effect_x, effect_y, 0);
    }
    if (is_poisoned) {
        float effect_x = shape->center_x() - (poison_animation->width * scale_x) / 2;
        float effect_y = shape->center_y() + (current_animation->height * scale_x) / 2 - (poison_animation->height * scale_x);
        algif_draw_gif(poison_animation, effect_x, effect_y, 0);
    }

    if (shield_value > 0) {
        float effect_x = shape->center_x() - (shield_animation->width * scale_x) / 2;
        float effect_y = shape->center_y() - (shield_animation->height * scale_y) / 2;
        algif_draw_gif(shield_animation, effect_x, effect_y, 0);
    }

    if (tp_gif) {
        float effect_x = shape->center_x() - (teleport_animation->width * scale_x) / 2;
        float effect_y = shape->center_y() - (teleport_animation->height * scale_y) / 2;
        algif_draw_gif(teleport_animation, effect_x, effect_y, 0);
    }

    for (const auto& proj : projectiles) {
        bool facing_left = proj.velocity_x < 0; // 判斷子彈方向
        algif_draw_gif(bulletLEFT_animation, proj.x, proj.y, facing_left ? 0 : ALLEGRO_FLIP_HORIZONTAL);
    }
}
// 設定藥水效果
void CharacterBase::set_effect_val(double hp, double sp_t, double sp_b, double atk_t, double atk_b){
    speed_bias = std::max(sp_b, speed_bias);
    HP = std::min(HP + hp, (double)1000);
    Speed_timer += sp_t;
    Atk_bias = std::max(Atk_bias, atk_b);
    Atk_timer += atk_t;
    if (hp > 0) {Hp_timer += 0.3;}
}
// 選角後的更新路徑
void CharacterBase::reset_gif_paths(const std::map<CharacterState, std::string>& new_gif_paths) {
    // 格式化並更新 GIF 路徑
    gifPath.clear();
    for (const auto& [state, base_path] : new_gif_paths) {
        gifPath[state] = base_path;
    }
    
    // 重置當前動畫為靜止狀態的 GIF
    GIFCenter* GIFC = GIFCenter::get_instance();
    current_animation = GIFC->get(gifPath[CharacterState::STOP]);
    update_bounding_box();
}

double CharacterBase::_get_HP()const{return HP;}
double CharacterBase::_get_Rage() const {return Rage;}

CharacterState CharacterBase::_get_state(){
    return state;
}
bool CharacterBase::_get_dir(){
    return is_facing_left;
}
double CharacterBase::_get_ATKtimer()const{
    return attack_timer;
}

double CharacterBase::_set_HP(double hp){
    if (shield_value > 0) shield_value += hp;
    else HP += hp;
}
double CharacterBase::_set_Rage(double rage){
    Rage += rage;
}
 double CharacterBase::_get_ATKbias(){
    return Atk_bias;
 }

void CharacterBase::attack_opponent(CharacterBase &opp){ // no use
    //opp.HP = std::max((double)0, (double)(opp.HP - 40));
    //opp.Rage = std::max((double) 0, (double)(opp.Rage + 15));
    //std::cout << "opp Hp: " << opp.HP  << " ,Rage: " << opp.Rage <<std::endl;
}

void CharacterBase::start_knockback(double distance, double direction) {
    this->sliding = true;          // 設置滑行狀態
    this->slide_distance = distance; // 設置總滑行距離
    this->slide_direction = direction; // 設置滑行方向
    this->slide_speed = distance / 60.0; // 假設滑行持續 1 秒，每幀移動距離
}

void CharacterBase::update_knockback() {
    if (!sliding) return; // 如果不在滑行狀態則直接返回

    double slide_step = std::min(slide_speed, slide_distance);
    shape->update_center_x(shape->center_x() + slide_direction * slide_step);

    slide_distance -= slide_step; // 減少剩餘滑行距離
    if (slide_distance <= 0) {
        sliding = false; // 滑行結束
        std::cout << "Knockback completed! Final position: " << shape->center_x() << std::endl;
    }
}

void CharacterBase::set_slide_timer(double t) {
    slide_timer = t;
}

void CharacterBase::_set_poisonTimer(double t){
    poison_timer = t;
    is_poisoned = true;
}

void CharacterBase::_set_Rage_status(bool b){
    is_Rage_status = b;
}

void CharacterBase::_set_Slowdown(bool b, double t){
    is_slow_down = true;
    slow_down_timer = t;
}
//冰凍
void CharacterBase::_set_freeze(bool frozen, double duration) {
    is_frozen = frozen;
    freeze_timer = duration;
    if (frozen) {
        set_state(CharacterState::FREEZE); // 凍住時角色狀態設為停止
    }
}

void CharacterBase::update_freeze() {
    if (is_frozen) {
        freeze_timer -= 1.0 / 60.0; // 每幀減少凍住時間（假設 60FPS）
        if (freeze_timer <= 0) {
            is_frozen = false; // 凍住結束
            std::cout << "Freeze effect ended. Target is now free to move.\n";
            set_state(CharacterState::STOP);
        } else {
            // 確保狀態保持在 FREEZE
            if (state != CharacterState::FREEZE) {
                set_state(CharacterState::FREEZE);
            }
        }
    }
}

bool CharacterBase::_is_frozen() const {
    return is_frozen;
}

//護盾
void CharacterBase::_set_shield(double shield_value, double duration) {
    this->shield_value = shield_value;
    this->shield_timer = duration;
    this->is_shielded = true;
    std::cout << "Shield activated with " << shield_value << " HP for " << duration << " seconds.\n";
}

void CharacterBase::update_shield() {
    if (is_shielded) {
        shield_timer -= 1.0 / 60.0; // 假設 60FPS
        if (shield_timer <= 0 || shield_value <= 0) {
            is_shielded = false; // 護盾結束
            shield_value = 0;
            std::cout << "Shield expired.\n";
        }
    }
}

//重製所有參數
void CharacterBase::reset() {
    HP = 1000;
    Rage = 0;
    state = CharacterState::STOP;
    is_facing_left = false;
    is_attacking = false;
    is_jumping = false;
    is_hurting = false;
    is_poisoned = false;
    sliding = false;
    is_slow_down = false;
    is_shielded = false;
    is_frozen = false;
    attack_timer = 0.0;
    shield_timer = 0.0;
    hurt_timer = 0.0;
    tp_gif_timer = 0.0;

    // 重置位置
    shape->update_center_x(initial_x);
    shape->update_center_y(initial_y);

    // 重新加載動畫
    GIFCenter* GIFC = GIFCenter::get_instance();
    current_animation = GIFC->get(gifPath[CharacterState::STOP]);
}

void CharacterBase::_set_tp_timer(double t) {
    tp_gif = true;
    tp_gif_timer = t;
}

void CharacterBase::shoot(double time) {
    double start_x = shape->center_x();
    double start_y = shape->center_y() - 50; // 子彈略高於角色中心
    double direction = _get_dir() ? -1.0 : 1.0;

    double velocity_x = 200.0 * direction;

    projectiles.push_back({
        start_x,
        start_y,
        velocity_x,
        0.0,
        time,
        std::make_unique<Rectangle>(
            start_x,
            start_y,
            start_x+bulletLEFT_animation->width,
            start_y+bulletLEFT_animation->height // 子彈大小為 20x20
        )
    });

    std::cout << "Projectile fired at (" << start_x << ", " << start_y 
              << ") with velocity (" << velocity_x << ", 0.0)\n";
}

void CharacterBase::update_projectiles() {
    std::vector<Projectile> active_projectiles;

    for (auto& proj : projectiles) {
        proj.x += proj.velocity_x * (1.0 / 60.0);
        proj.y += proj.velocity_y * (1.0 / 60.0);
        proj.lifetime -= 1.0 / 60.0;

        // 更新碰撞箱的位置
        proj.shape->update_center_x(proj.x);
        proj.shape->update_center_y(proj.y);

        if (proj.lifetime > 0) {
            active_projectiles.push_back(std::move(proj));
        }
    }

    projectiles = std::move(active_projectiles);
}

void CharacterBase::_set_sprint_flag(bool b){
    sprint_dective = b;
}
bool CharacterBase::_get_sprint_flag(){
    return sprint_dective;
}

void CharacterBase::sprint_init(int dir, double distance, CharacterState nxt_state){
    ATK_flag = true;
    atk_flag_state = nxt_state;
    is_attacking = false;
    attack_timer = 0;
    is_sprint = true;
    sprint_dective = true;
    sprint_timer = 1.0;
    sprint_dir = dir;
    sprint_distance = distance;
    sprint_speed = sprint_distance / 60.0; 
    set_state(CharacterState::RUN);
}

double CharacterBase::_get_sprint_timer(){
    return sprint_timer;
}
void CharacterBase::sprint_update(){
    if (!is_sprint)return;

    double now_step = std::min(sprint_distance, sprint_speed);
    shape->update_center_x(shape->center_x() + now_step*sprint_dir);
    sprint_distance -= now_step;
    if (sprint_distance <= 0){
        is_sprint = false;
        sprint_dective = false;
        if (ATK_flag){
            is_attacking = true;
            set_state(atk_flag_state);
            attack_timer = attack_duration;
            ATK_flag = false;
            
        }
    }
}
