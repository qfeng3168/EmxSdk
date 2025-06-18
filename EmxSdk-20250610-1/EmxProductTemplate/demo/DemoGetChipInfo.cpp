//
// Created by tangfahong on 2024/4/29.
//

#include "EmxMedia.hpp"
#include "media/client/MediaSystem.hpp"

using namespace Emx;
// 视频隐私遮挡配置示例
int main(int argc, char **argv) {
    MediaSystem sys;
    std::string root;
    auto e = sys.GetChipInfo(root);
    emxlogi("e=%d,root=%s\n",e,root.c_str());
    return 0;
}