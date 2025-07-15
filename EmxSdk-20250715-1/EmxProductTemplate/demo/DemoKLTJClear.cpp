//
// Created by tangfahong on 2024/4/22.
//
#include "EmxMedia.hpp"

using namespace Emx;

int main(int argc, char **argv) {
   
    MediaAi demo(0);
    Json::Value clear;
    Json::Value out;
    clear["passengerFlowStatistics"]["clear"] = true;
    ErrCodeE e = demo.Ctrl(clear,out);
    printf("unregister ret=%d \n",(int)e);

    return 0;
}