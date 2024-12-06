#include "Prop_GOD.h"
#include "../data/DataCenter.h"
#include <iostream>

/*
class Prop_God: public Object{
    public:
    Prop_God();
    void draw(); // 0
    void update(); // gen


    private:
        int gen_rate = 10;
        int time_cnt = 0;
};
*/


Prop_God::Prop_God(int gr): gen_rate{gr} {}
void Prop_God::draw(){
    return;
}

void Prop_God::update(){
    if (time_cnt < gen_rate){
        time_cnt++;
        return;
    }
    time_cnt = 0;
    DataCenter *DC = DataCenter::get_instance();
    DC->props.emplace_back(Prop::Prop_Create(Point{0, 0}, Point{300, 300}, PropsType::Healthy, 50, 100));

    //assert("props vector size: ",DC->props.size());
    std::cout << "Prop vector size: " << DC->props.size() <<"\n";
    return;
}