#ifndef PORP_GOD_H_INCLUDED
#define PORP_GOD_H_INCLUDED

#include "Props.h"
#include "../Object.h"
#include <vector>

class Prop_God: public Object{
    public:
    Prop_God(int);
    void draw(); // 0
    void update(); // gen


    private:
        int gen_rate = 10;
        int time_cnt = 0;
        std::vector<int> Init_x;
        int idx_x = 0;
};

#endif 

