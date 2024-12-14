#include "Prop_GOD.h"
#include "../data/DataCenter.h"
#include <iostream>
#include <random>
#include <ctime>
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
void Prop_God::reset(){
    Init_x.clear();
    std::random_device rd; // 真正隨機種子（硬體支持）
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<int> dist(1, 1600); // 範圍 [1, 100]
    int x;
    for (int i=0;i<1000;i++){
        x = dist(engine);
        Init_x.push_back(x);
    }
}

Prop_God::Prop_God(int gr): gen_rate{gr} {
    std::random_device rd; // 真正隨機種子（硬體支持）
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<int> dist(1, 1600); // 範圍 [1, 100]
    int x;
    for (int i=0;i<1000;i++){
        x = dist(engine);
        Init_x.push_back(x);
    }
}
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
    if (DC->props.size() >= 10)return;
    
    //std::random_device rd; // 真正隨機種子（硬體支持）
    
    int init_x = Init_x[idx_x++];
    idx_x %= Init_x.size();
    switch(init_x % 3){
        case 0:
            DC->props.emplace_back(Prop::Prop_Create(Point{init_x, -200}, Point{init_x , 700}, PropsType::Healthy, 50, 200));
            break;
         case 1:
            DC->props.emplace_back(Prop::Prop_Create(Point{init_x, -200}, Point{init_x , 700}, PropsType::Haste, 50, 10));
            break;
        case 2:
            DC->props.emplace_back(Prop::Prop_Create(Point{init_x, -200}, Point{init_x , 700}, PropsType::Crit, 50, 60));
            break;
    }
    // static Prop *Prop_Create(const Point &p, const Point &target, PropsType Type , double v, double val);
    
    
    //assert("props vector size: ",DC->props.size());
    //std::cout << "Prop vector size: " << DC->props.size() <<"\n";
    return;
}

void Prop_God::clear_all_props() {
    DataCenter *DC = DataCenter::get_instance();
    DC->props.clear(); // 清空道具向量
    std::cout << "All props have been cleared!" << std::endl;
}