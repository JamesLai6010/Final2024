#include "Meteor.h"
#include "../data/DataCenter.h"
#include <iostream>

void Meteor::generate(double startX, double startY, double velocityY, double lifetime) {
    // 隕石從屏幕上方生成
    x = startX;
    y = startY;
    velocity_y = velocityY;
    this->lifetime = lifetime;

    // 初始化隕石的碰撞箱 (大小可以調整)
    shape = std::make_unique<Rectangle>(
        x, y,
        x + width,
        y + height
    );

    std::cout << "Meteor generated at (" << x << ", " << y 
              << ") with velocity (0.0, " << velocity_y << ")\n";
}

void Meteor::update() {
    // 隕石下落
    y += velocity_y * (1.0 / 60.0);
    lifetime -= 1.0 / 60.0;

    // 更新碰撞箱的位置
    shape->update_center_x(x + width / 2.0);
    shape->update_center_y(y + height / 2.0);

    // 標記過期的隕石
    if (lifetime <= 0 || y > 1080) {
        toDelete = true;
        std::cout << "Meteor expired at (" << x << ", " << y << ")\n";
    }
}

void Meteor::draw() {
    // 根據切換計時器顯示不同的圖片
    if (showFirstImage) {
        al_draw_bitmap(image1, x, y, 0);
    } else {
        al_draw_bitmap(image2, x, y, 0);
    }

    // 計時器更新（閃爍效果）
    timer++;
    if (timer >= 15) {
        showFirstImage = !showFirstImage;
        timer = 0;
    }
}
