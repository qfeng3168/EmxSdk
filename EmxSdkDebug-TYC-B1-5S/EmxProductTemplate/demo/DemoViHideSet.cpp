//
// Created by tangfahong on 2024/4/29.
//

#include "EmxMedia.hpp"

using namespace Emx;

// 视频隐私遮挡配置示例
int main(int argc, char **argv) {
    // 用于存放参数数据
    MediaVi::Param param = {};
    MediaVi vi(0);
    // 先获取当前的配置
    if (vi.GetParam(param) != ErrCodeE::Success) {
        emxloge("GetParam vi failed\n");
        return -1;
    }
    if(atoi(argv[1])){
        param.hide = true;
    }else{
        param.hide = false;
    }

    if (vi.SetTmpParam(param) != ErrCodeE::Success) {
        emxloge("SetParam failed\n");
        return -1;
    }
    emxlogi("SetTmpParam vi success\n");
    return 0;
}