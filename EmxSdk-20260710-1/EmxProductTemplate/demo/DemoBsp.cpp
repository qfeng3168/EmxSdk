//
// Created by tangfahong on 2025/3/19.
//


#include "modules/Bsp.hpp"
using namespace Emx;


int main() {
    Emx::Bsp bsp;
    emxlogi("get adc value = %d \n",bsp.adc->GetAdcValue(0));

    return 0;
}