//
// Created by tangfahong on 2025/3/19.
//


#include "modules/Bsp.hpp"
using namespace Emx;


int main() {
    Emx::Bsp bsp;
    emxlogi("get rca status = %d \n",bsp.rcaDetect->Get());   

    return 0;
}