//
// Created by xiong on 2023/8/29.
//

#include "EmxMedia.hpp"

using namespace Emx;

// 视频编码配置示例，音频/ISP等使用SetParam/GetParam接口进行参数配置的类都可参考此示例
int main() {
// 用于存放参数数据
    MediaVenc::Param param = {};
    // 定义一个通道0的编码器对象
    MediaVenc venc(0);
    // 先获取当前的配置
    if (venc.GetParam(param) != ErrCodeE::Success) {
        emxloge("GetParam failed\n");
        return -1;
    }

    // 切换编码格式
    if (param.codec == Emx::VideoCodecE::H265)
        param.codec = VideoCodecE::H264;
    else
        param.codec = Emx::VideoCodecE::H265;

    if (venc.SetParam(param) != ErrCodeE::Success) {
        emxloge("SetParam failed\n");
        return -1;
    }
    emxlogi("SetParam success\n");
    return 0;
}