//
// Created by tangfahong on 2025/3/19.
//


#include "modules/Bsp.hpp"
using namespace Emx;


int main(int argc, char *argv[]) {
    Emx::Bsp bsp;
    int gpio = 0;
    int dir = 0;
    int value = 0;
    gpio = atoi(argv[1]);
    dir = atoi(argv[2]);
    if(argc > 3) {
        value = atoi(argv[3]);
    }
    bsp.gmGpio->SetDir(gpio, dir);
    if(dir == 1) {
        bsp.gmGpio->Set(gpio, value);
    }else{
        value = (int)bsp.gmGpio->Get(gpio);
    }

    emxlogi("gpio:%d,dir;%d value:%d \n", gpio,dir, value);   

    return 0;
}