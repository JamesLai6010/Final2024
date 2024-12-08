
#include "GameInformation.h"
#include "../data/DataCenter.h"
#include "../data/GIFCenter.h"
#include "../Character/Character1.h"
#include "../Character/CharacterBase.h"
#include "../Character/Character2.h"
#include <random>
//#include "../algif5/algif.h"



constexpr char heart_gif_path[] = "./assets/gif/heart.gif";
constexpr char bolt_img_path[] = "./assets/image/bolt.png";

void BackgroundINF::init() {
    ImageCenter *IC = ImageCenter::get_instance();
    GIFCenter *GC = GIFCenter::get_instance();
    bolt = IC->get(bolt_img_path);
    heart = GC->get(heart_gif_path);
}

BackgroundINF::BackgroundINF(int t){ // min
    
    Time = t*60;

    player1_HP = max_HP;
    player2_HP = max_HP;
    Player1_Rage = 0;
    Player2_Rage = 0;
    std::random_device rd; // 真正隨機種子（硬體支持）
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<int> dist(1, 10000); // 範圍 [1, 100]
    for (int i=0;i<10000;i++){
        Random_Num.push_back(dist(engine));
    }
    rand_idx = 0;

}

void BackgroundINF::update(){
    
    DataCenter *DC = DataCenter::get_instance();
    CharacterBase &CH1 = *(DC->character1);
    CharacterBase &CH2 = *(DC->character2);
    player1_HP = CH1._get_HP();
    player2_HP = CH2._get_HP();
    Player1_Rage = CH1._get_Rage();
    Player2_Rage = CH2._get_Rage();
    Time--;
}

void BackgroundINF::draw(){
    
    double ch1_healthy_percentage = player1_HP /  1000;
    double ch2_healthy_percentage = player2_HP /  1000;
    double ch1_Rage_percentage = Player1_Rage /  100;
    double ch2_Rage_percentage = Player2_Rage /  100;

    double rage_bar = bar_width * 0.8;
    int ch1_health_bar = bar_width * ch1_healthy_percentage;
    int ch2_health_bar = bar_width * ch2_healthy_percentage;
    int ch1_rage_bar = rage_bar * ch1_Rage_percentage;
    int ch2_rage_bar = rage_bar * ch2_Rage_percentage;
    al_init_primitives_addon();
    
    
    // draw ch1 inf.
    double ch1_healthBar_center_x = 100 + bar_width / 2.0;
    double ch1_healthBar_center_y = 30 + bar_height / 2.0;
    al_draw_filled_rectangle(60-5, 30-5, 60+bar_width+5, 30 + bar_height+5, al_map_rgb(255, 255, 255));
    al_draw_filled_rectangle(60, 30, 60+ch1_health_bar, 30 + bar_height, al_map_rgb(255, 20, 20));
    al_draw_filled_rectangle(60-5, 30+bar_height, 60 + rage_bar+5, 30+bar_height+bar_height+10, al_map_rgb(255, 255, 255));
    al_draw_filled_rectangle(60, 30+bar_height+5, 60 + rage_bar, 30+bar_height+bar_height+5, al_map_rgb(128, 128, 128));
    al_draw_filled_rectangle(60, 30+bar_height+5, 60 + ch1_rage_bar, 30+bar_height+bar_height+5, al_map_rgb(20, 20, 255));
    algif_draw_gif(heart, 0, ch1_healthBar_center_y - heart->height / 2, 0);
    al_draw_bitmap(bolt, 7, ch1_healthBar_center_y * 2 - heart->height / 2, 0);
    // draw ch2 inf.
    double ch2_healthBar_center_x = 1500 - bar_width / 2.0;
    double ch2_healthBar_center_y = 30 + bar_height / 2.0;
    al_draw_filled_rectangle(1540 - bar_width-5, 30-5, 1540+5, 30 + bar_height+5, al_map_rgb(255, 255, 255));
    al_draw_filled_rectangle(1540 - ch2_health_bar, 30, 1540, 30 + bar_height, al_map_rgb(255, 20, 20));
    al_draw_filled_rectangle(1540 - rage_bar-5, 30+bar_height, 1540+5, 30+bar_height+bar_height+10, al_map_rgb(255, 255, 255));
    al_draw_filled_rectangle(1540 - rage_bar, 30+bar_height+5, 1540, 30+bar_height+bar_height+5, al_map_rgb(128, 128, 128));
    al_draw_filled_rectangle(1540 - ch2_rage_bar, 30+bar_height+5, 1540, 30+bar_height+bar_height+5, al_map_rgb(20, 20, 255));
    algif_draw_gif(heart, 1600-heart->width, ch1_healthBar_center_y - heart->height / 2, 0);
    al_draw_bitmap(bolt, 1600-heart->width, ch1_healthBar_center_y * 2 - heart->height / 2, 0);

    
}
/*
	al_draw_bitmap(
		icon,
		shape->center_x() - al_get_bitmap_width(icon) / 2,
		shape->center_y() - al_get_bitmap_height(icon) / 2, 0); // 左上角
*/

int BackgroundINF::_get_random_num(){
    int x = Random_Num[rand_idx];
    rand_idx = (rand_idx+1) % Random_Num.size();
    return x;
}
