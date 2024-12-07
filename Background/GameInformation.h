#ifndef GAMEINF_H_INCLUDED
#define GAMEINF_H_INCLUDED

#include "../Object.h"


class BackgroundINF: public Object{
    public:
    BackgroundINF(int x); // 設定每回合的時間
    void draw(); // 0
    void update(); // gen


    private:
        double player1_HP;
        double player2_HP;
        double Player1_Rage;
        double Player2_Rage;
        int Time = 0;

        double bar_width = 300;
        double bar_height = 40;
        

        double max_HP = 1000;
};

#endif 

