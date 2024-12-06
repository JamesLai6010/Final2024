#include "Character1.h"
#include "../data/DataCenter.h"
#include "../algif5/algif.h"
#include <stdexcept>
#include "../shapes/Rectangle.h"
#include "../data/GIFCenter.h"

namespace Character1Setting {
    static constexpr char gif_root_path[] = "./assets/gif/Character1";
    static constexpr char gif_postfix[][10] = {
        "Walk", "Run", "Jump", "Attack1", "Attack2", "Attack3", "Stop", "Shield", "Dead"
    };
}
void Character1::update_bounding_box() {
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


void Character1::set_scale(double sx, double sy) {
    scale_x = sx;
    scale_y = sy;
    update_bounding_box(); // 縮放後同步更新碰撞箱
}

void Character1::init() {
    GIFCenter* GIFC = GIFCenter::get_instance();
    DataCenter* DC = DataCenter::get_instance();

    // 初始化各模式的 GIF 路徑
    for (size_t i = 0; i < static_cast<size_t>(Character1State::NONE); ++i) {
        char buffer[100];
        sprintf(buffer, "%s/%s.gif",
                Character1Setting::gif_root_path,
                Character1Setting::gif_postfix[i]);
        gifPath[static_cast<Character1State>(i)] = std::string(buffer);
    }

    // 加載初始動畫 (靜止)
    current_animation = GIFC->get(gifPath[Character1State::STOP]);

    float initial_x = DC->window_width / 2;  // 水平位置為畫面中心
    float initial_y = ground_level;         // 垂直位置為地面高度

    // 初始化角色形狀和位置
    shape.reset(new Rectangle(
        initial_x - current_animation->width / 2,  // 左上角 X
        initial_y - current_animation->height / 2,     // 左上角 Y
        initial_x + current_animation->width / 2,  // 右下角 X
        initial_y + current_animation->height / 2    // 右下角 Y
    ));

    // 同步更新碰撞框
    update_bounding_box();
}

void Character1::set_state(Character1State new_state) {
    if (state != new_state) {
        state = new_state;
        GIFCenter* GIFC = GIFCenter::get_instance();

        // 切換到新狀態的 GIF
        current_animation = GIFC->get(gifPath[state]);
        update_bounding_box(); // 更新碰撞箱
    }
}



void Character1::update() {
    DataCenter* DC = DataCenter::get_instance();

    // 攻擊狀態計時
    if (is_attacking) {
        attack_timer -= 1.0 / 60.0; // 減少攻擊計時器
        if (attack_timer <= 0) {
            is_attacking = false; // 攻擊結束
            set_state(Character1State::STOP); // 返回停止狀態
        }
    }

    // **防止在 SHIELD 狀態下水平移動**    
    if (state != Character1State::SHIELD) {
        // 水平移動處理（即使在攻擊狀態下仍允許移動）
        if (DC->key_state[ALLEGRO_KEY_D]) {
            shape->update_center_x(shape->center_x() + speed);
            is_facing_left = false; // 面向右
            if (!is_attacking && !is_jumping) {
                set_state(Character1State::WALK);
            }
        } else if (DC->key_state[ALLEGRO_KEY_A]) {
            shape->update_center_x(shape->center_x() - speed);
            is_facing_left = true; // 面向左
            if (!is_attacking && !is_jumping) {
                set_state(Character1State::WALK);
            }
        } else if (!is_attacking && !is_jumping) {
            set_state(Character1State::STOP); // 停止狀態
        }
    }
    

    // 攻擊和防禦邏輯處理（按鍵對應不同動畫）
    if (!is_attacking) {
        if (DC->key_state[ALLEGRO_KEY_F]) {
            set_state(Character1State::ATTACK1);
            is_attacking = true;
            attack_timer = attack_duration; // 設定 ATTACK1 動畫持續時間
        } else if (DC->key_state[ALLEGRO_KEY_G]) {
            set_state(Character1State::ATTACK2);
            is_attacking = true;
            attack_timer = attack_duration; // 設定 ATTACK2 動畫持續時間
        } else if (DC->key_state[ALLEGRO_KEY_H]) {
            set_state(Character1State::ATTACK3);
            is_attacking = true;
            attack_timer = attack_duration; // 設定 ATTACK3 動畫持續時間
        } else if (DC->key_state[ALLEGRO_KEY_J]) {
            set_state(Character1State::SHIELD);
            is_attacking = true;
            attack_timer = shield_duration; // 設定防禦持續時間
        }
    }
    // 處理跳躍邏輯
    if (is_jumping) {
        // 更新垂直速度（加入重力影響）
        vertical_velocity += gravity * (1.0 / 60.0); // 假設 60FPS
        shape->update_center_y(shape->center_y() + vertical_velocity); // 根據速度更新垂直位置

        // 檢查是否到達地面
        if (shape->center_y() >= ground_level) {
            shape->update_center_y(ground_level); // 修正位置到地面
            is_jumping = false;                  // 停止跳躍狀態
            vertical_velocity = 0;               // 重置速度
            set_state(Character1State::STOP);    // 回到停止狀態
        }
    }

    // 處理跳躍按鍵
    if (!is_jumping && DC->key_state[ALLEGRO_KEY_W]) {
        is_jumping = true;
        vertical_velocity = -jump_initial_velocity; // 跳躍初速度（負值表示向上）
        set_state(Character1State::JUMP);  // 切換到跳躍狀態
    }
}


void Character1::draw() {
    if (!current_animation) return;

    float draw_x = shape->center_x() - (current_animation->width * scale_x) / 2;
    float draw_y = shape->center_y() - (current_animation->height * scale_y) / 2;

    int flags = is_facing_left ? ALLEGRO_FLIP_HORIZONTAL : 0;

    // 繪製當前幀
    algif_draw_gif(
        current_animation,
        draw_x,
        draw_y,
        flags
    );
}