
#include "GameInformation.h"
#include "../data/DataCenter.h"
#include "../Character/Character1.h"
#include "../Character/CharacterBase.h"
#include "../Character/Character2.h"
//#include "../algif5/algif.h"
#include <allegro5/allegro_primitives.h>

/*
class GameINF: public Object{
    public:
    GameINF(int); // 設定每回合的時間
    void draw(); // 0
    void update(); // gen


    private:
        double player1_HP;
        double player2_HP;
        double Player1_Rage;
        double Player2_Rage;
        int Time = 0;
};
*/

BackgroundINF::BackgroundINF(int t){ // min
    Time = t*60;

    player1_HP = max_HP;
    player2_HP =max_HP;
    Player1_Rage = 0;
    Player2_Rage = 0;
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
    al_draw_filled_rectangle(0, 30, 0+bar_width, 30 + bar_height, al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(0, 30, 0+ch1_health_bar, 30 + bar_height, al_map_rgb(255, 20, 20));
    al_draw_filled_rectangle(0, 30+bar_height, 0 + rage_bar, 30+bar_height+bar_height, al_map_rgb(128, 128, 128));
    al_draw_filled_rectangle(0, 30+bar_height, 0 + ch1_rage_bar, 30+bar_height+bar_height, al_map_rgb(20, 20, 255));
    // draw ch2 inf.
    al_draw_filled_rectangle(1600 - bar_width, 30, 1600, 30 + bar_height, al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(1600 - ch2_health_bar, 30, 1600, 30 + bar_height, al_map_rgb(255, 20, 20));
    al_draw_filled_rectangle(1600 - rage_bar, 30+bar_height, 1600, 30+bar_height+bar_height, al_map_rgb(128, 128, 128));
    al_draw_filled_rectangle(1600 - ch2_rage_bar, 30+bar_height, 1600, 30+bar_height+bar_height, al_map_rgb(20, 20, 255));

    
}
/*
	al_draw_bitmap(
		icon,
		shape->center_x() - al_get_bitmap_width(icon) / 2,
		shape->center_y() - al_get_bitmap_height(icon) / 2, 0); // 左上角
*/
