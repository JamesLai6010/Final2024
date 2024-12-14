#ifndef METEOR_H
#define METEOR_H

#include "../shapes/Rectangle.h"
#include <memory>
#include <allegro5/allegro.h>

class Meteor {
public:
    double x, y;                     // 隕石位置
    double velocity_y;               // 垂直速度
    double lifetime;                 // 隕石存活時間
    int width, height;               // 隕石大小
    bool toDelete = false;           // 是否需要刪除
    int timer = 0;                   // 用於圖片切換的計時器
    bool showFirstImage = true;      // 控制顯示哪一張圖片
    ALLEGRO_BITMAP* image1;          // 隕石圖片 1
    ALLEGRO_BITMAP* image2;          // 隕石圖片 2
    std::unique_ptr<Rectangle> shape; // 隕石的碰撞箱

    // 隕石生成
    void generate(double startX, double startY, double velocityY, double lifetime);

    // 更新隕石位置
    void update();

    // 繪製隕石
    void draw();
};


#endif
