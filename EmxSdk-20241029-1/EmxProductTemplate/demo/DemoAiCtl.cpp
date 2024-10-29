//
// Created by tangfahong on 2024/4/22.
//
#include "EmxMedia.hpp"

using namespace Emx;

int main(int argc, char **argv) {
    MediaAi demo(0);
    Json::Value library;
    Json::Value out;
    if (EasyJson::Load(argv[1], library) != ErrCodeE::Success) {
        printf("load %s failed\n", argv[1]);
        return -1;
    }
    ErrCodeE e = demo.Ctrl(library,out);
    printf("Ai ctrl ret=%d \n",(int)e);
    return 0;
}