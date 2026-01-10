//
// Created by tangfahong on 2025/6/10.
//
#include "EmxMedia.hpp"

using namespace Emx;

// Ptz使用示例
int main(int argc, char **argv) {
    if (argc < 2) {
        emxloge("usage: %s <status> :\n \
            VideoFrameSuccessful = 0,//!<完整视频帧获取成功\
            VideoFrameFailed,        //!<完整视频帧获取失败\
            TargetTrigger,           //!<目标触发（人形侦测触发）\
            TargetLost,              //!<目标丢失（人形侦测丢失）\
            \n", argv[0]);
        return -1;
    }
    MediaAov aov(0);
    aov.UpdateStatus((MediaInfAov::AovStatusTypeE)atoi(argv[1]));
    return 0;
}