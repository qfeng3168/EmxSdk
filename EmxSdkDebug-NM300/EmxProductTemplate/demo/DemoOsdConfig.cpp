//
// Created by xiong on 2023/8/29.
//

#include "EmxMedia.hpp"

using namespace Emx;

// OSD配置示例
int main() {
// 用于存放参数数据
    MediaOsd::Param param = {};
    // 定义一个通道0的编码器对象
    MediaOsd osd(0);
    // 先获取当前的配置
    if (osd.GetParam(param) != ErrCodeE::Success) {
        emxloge("GetParam failed\n");
        return -1;
    }

    // 假设这里需要修改时间戳的对齐方式
    for (auto &item: param.array) {
        if (item->type == MediaOsd::TypeE::Time) {
            if (item->margin.horizon == MediaOsd::HAlignE::Left)
                item->margin.horizon = MediaOsd::HAlignE::Right;
            else
                item->margin.horizon = MediaOsd::HAlignE::Left;
        }
    }

    if (osd.SetParam(param) != ErrCodeE::Success) {
        emxloge("SetParam failed\n");
        return -1;
    }
    emxlogi("SetParam success\n");
    return 0;
}