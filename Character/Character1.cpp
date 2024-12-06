#include "Character1.h"
#include "../data/DataCenter.h"
#include "../algif5/algif.h"
#include <stdexcept>
#include "../shapes/Rectangle.h"
#include "../data/ImageCenter.h"

constexpr char character1_img_path[] = "./assets/image/Character1/Walk.png";

void Character1::update_bounding_box() {
    if (shape) {
        float center_x = shape->center_x();
        float center_y = shape->center_y();

        float new_width = animations[state].frame_width * scale_x;
        float new_height = animations[state].frame_height * scale_y;

        // 如果角色翻转，需要调整碰撞箱的位置
        //float offset_x = is_facing_left ? -new_width / 2 : new_width / 2;

        // 更新碰撞箱
        shape.reset(new Rectangle(center_x - new_width / 2, center_y - new_height / 2,
                          center_x + new_width / 2, center_y + new_height / 2));

    }
}


void Character1::set_scale(double sx, double sy) {
    scale_x = sx;
    scale_y = sy;
    update_bounding_box(); // 縮放後同步更新碰撞箱
}

void Character1::init() {
    
    ImageCenter *IC = ImageCenter::get_instance();
    DataCenter *DC = DataCenter::get_instance();
    // 加載多幀圖片
    ALLEGRO_BITMAP *sprite_sheet = IC->get(character1_img_path);
    //GAME_ASSERT(sprite_sheet != nullptr, "Failed to load Walk sprite sheet!");

    // 設置動畫幀數
    animations[Character1State::WALK].sprite_sheet = sprite_sheet;
    animations[Character1State::WALK].frame_width = 128;  // 單幀寬度
    animations[Character1State::WALK].frame_height = 128; // 單幀高度
    animations[Character1State::WALK].num_frames = 8;    // 總幀數

    // 設置角色初始位置和碰撞框
    shape.reset(new Rectangle{DC->window_width / 2,
							  DC->window_height / 2,
							  DC->window_width / 2 + 128,
							  DC->window_height / 2 + 128});

    // 更新碰撞箱（初始化時必須同步）
    update_bounding_box();
}

void Character1::set_state(Character1State new_state) {
    if (state != new_state) {
        state = new_state;
        frame_index = 0;   // 重置幀索引
        frame_timer = 0.0; // 重置幀計時器
    }
}

void Character1::update() {
    DataCenter *DC = DataCenter::get_instance();
    // 更新角色位置
    is_moving = false;
    if (DC->key_state[ALLEGRO_KEY_D]) { // 往右移動
        shape->update_center_x(shape->center_x() + speed);
        set_state(Character1State::WALK);
        is_moving = true;
        is_facing_left = false; // 面向右
    } else if (DC->key_state[ALLEGRO_KEY_A]) { // 往左移動
        shape->update_center_x(shape->center_x() - speed);
        set_state(Character1State::WALK);
        is_moving = true;
        is_facing_left = true;  //面向左
    }

    if (!is_moving) {
        set_state(Character1State::STOP);
    }

    // 更新動畫幀
    if (state == Character1State::WALK) {
        frame_timer += 1.0 / 60.0; // 假設 FPS = 60
        if (frame_timer >= frame_duration) {
            frame_timer -= frame_duration;
            frame_index = (frame_index + 1) % animations[Character1State::WALK].num_frames;
        }
    }

    // 更新碰撞箱
    update_bounding_box();
}


void Character1::draw() {
    if (state == Character1State::NONE) return; // 停止時不繪製

    Animation &anim = animations[Character1State::WALK];
    int sx = frame_index * anim.frame_width; // 計算當前幀的 X 偏移量

    // 計算繪製的目標位置
    float draw_x = shape->center_x() - (anim.frame_width * scale_x) / 2;
    float draw_y = shape->center_y() - (anim.frame_height * scale_y) / 2;
    
    // 判斷是否需要翻轉圖片
    int flags = is_facing_left ? ALLEGRO_FLIP_HORIZONTAL : 0;

    // 使用縮放繪製
    al_draw_scaled_bitmap(
        anim.sprite_sheet,          // 原始圖片
        sx, 0,                      // 原圖片選取範圍 (左上角 x, y)
        anim.frame_width,           // 選取範圍的寬度
        anim.frame_height,          // 選取範圍的高度
        draw_x,                     // 繪製位置 x
        draw_y,                     // 繪製位置 y
        anim.frame_width * scale_x, // 繪製寬度（應用水平縮放）
        anim.frame_height * scale_y,// 繪製高度（應用垂直縮放）
        flags);                     // 翻轉參數
}


