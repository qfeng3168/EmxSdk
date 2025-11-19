//
// Created by tangfahong on 2025/6/10.
//
#include "EmxMedia.hpp"

using namespace Emx;

// Ptz使用示例
int main(int argc, char **argv) {
    if (argc < 3) {
        emxloge("usage: %s <mode> <intervalS>\n", argv[0]);
        return -1;
    }
    MediaAov::Param aovParam = {};
    MediaAov aov(0);
    aov.GetParam(aovParam);
    aovParam.mode = atoi(argv[1]) == 0 ? MediaInfAov::AovModeE::Normal : MediaInfAov::AovModeE::LowPower;
    aovParam.intervalS = atoi(argv[2]);
    if (aov.SetParam(aovParam) != ErrCodeE::Success) {
        emxloge("set aov info failed!\n");
        return -1;
    }
    return 0;
}