#ifndef GAMEINF_H_INCLUDED
#define GAMEINF_H_INCLUDED

#include "../Object.h"
#include "../data/ImageCenter.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "../algif5/algif.h"

class BackgroundINF: public Object{
    public:
    BackgroundINF(int x); // 設定每回合的時間
    void draw(); // 0
    void update(); // gen
    void init();


    private:
        double player1_HP;
        double player2_HP;
        double Player1_Rage;
        double Player2_Rage;
        int Time = 0;

        double bar_width = 448;
        double bar_height = 30;
        
        double max_HP = 1000;

        ALLEGRO_BITMAP *healthBar;
};

#endif 

