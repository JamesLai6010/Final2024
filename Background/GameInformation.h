#ifndef GAMEINF_H_INCLUDED
#define GAMEINF_H_INCLUDED

#include "../Object.h"
#include "../data/ImageCenter.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "../algif5/algif.h"
#include <vector> 
#include <ctime>

class BackgroundINF: public Object{
    public:
    BackgroundINF(int x); // 設定每回合的時間
    void draw(); // 0
    void update(); // gen
    void init();
    int _get_random_num();
    int get_Time();
    void _set_time();
    void _set_Time_flag(bool b);



    bool _get_Time_flag();

    private:
        double player1_HP;
        double player2_HP;
        double Player1_Rage;
        double Player2_Rage;
        int Time = 0;
        bool Time_change_flag = true;
        std::time_t start;
        double bar_width = 448;
        double bar_height = 30;
        
        double max_HP = 1000;

        ALLEGRO_BITMAP *bolt;
        ALGIF_ANIMATION *heart;

        std::vector<int> Random_Num;
        int rand_idx;

};

#endif 

