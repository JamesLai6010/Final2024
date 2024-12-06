#include "CharacterBase.h"
#include "../data/DataCenter.h"
#include "../algif5/algif.h"
#include <stdexcept>
#include "../shapes/Rectangle.h"
#include "../data/GIFCenter.h"

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

    // 初始化 GIF 路徑
    for (size_t i = 0; i < static_cast<size_t>(CharacterState::NONE); ++i) {
        char buffer[100];
        sprintf(buffer, "./assets/gif/%s.gif", gifPath[static_cast<CharacterState>(i)].c_str());
        gifPath[static_cast<CharacterState>(i)] = std::string(buffer);
    }

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
    if (state != new_state) {
        state = new_state;
        GIFCenter* GIFC = GIFCenter::get_instance();
        current_animation = GIFC->get(gifPath[state]);
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

     // **防止在 SHIELD 狀態下水平移動**    
    if (state != CharacterState::SHIELD) {
        // 水平移動處理（即使在攻擊狀態下仍允許移動）
        if (DC->key_state[key_right]) {
            shape->update_center_x(shape->center_x() + speed);
            is_facing_left = false; // 面向右
            if (!is_attacking && !is_jumping) {
                set_state(CharacterState::WALK);
            }
        } else if (DC->key_state[key_left]) {
            shape->update_center_x(shape->center_x() - speed);
            is_facing_left = true; // 面向左
            if (!is_attacking && !is_jumping) {
                set_state(CharacterState::WALK);
            }
        } else if (!is_attacking && !is_jumping) {
            set_state(CharacterState::STOP); // 停止狀態
        }
    }
    // 攻擊狀態計時
    if (is_attacking) {
        attack_timer -= 1.0 / 60.0; // 減少攻擊計時器
        if (attack_timer <= 0) {
            is_attacking = false; // 攻擊結束
            set_state(CharacterState::STOP); // 返回停止狀態
        }
    }
    // 處理攻擊按鍵
    // 攻擊和防禦邏輯處理（按鍵對應不同動畫）
    if (!is_attacking) {
        if (DC->key_state[key_attack1]) {
            set_state(CharacterState::ATTACK1);
            is_attacking = true;
            attack_timer = attack_duration; // 設定 ATTACK1 動畫持續時間
        } else if (DC->key_state[key_attack2]) {
            set_state(CharacterState::ATTACK2);
            is_attacking = true;
            attack_timer = attack_duration; // 設定 ATTACK2 動畫持續時間
        } else if (DC->key_state[key_attack3]) {
            set_state(CharacterState::ATTACK3);
            is_attacking = true;
            attack_timer = attack_duration; // 設定 ATTACK3 動畫持續時間
        } else if (DC->key_state[key_shield]) {
            set_state(CharacterState::SHIELD);
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
            set_state(CharacterState::STOP);    // 回到停止狀態
        }
    }

    // 處理跳躍按鍵
    if (!is_jumping && DC->key_state[key_jump]) {
        is_jumping = true;
        vertical_velocity = -jump_initial_velocity; // 跳躍初速度（負值表示向上）
        set_state(CharacterState::JUMP);  // 切換到跳躍狀態
    }
}

void CharacterBase::draw() {
    if (!current_animation) return;

    float draw_x = shape->center_x() - (current_animation->width * scale_x) / 2;
    float draw_y = shape->center_y() - (current_animation->height * scale_y) / 2;

    int flags = is_facing_left ? ALLEGRO_FLIP_HORIZONTAL : 0;

    algif_draw_gif(current_animation, draw_x, draw_y, flags);
}
