//
// Created by xiong on 2023/8/29.
//

#include "EmxMedia.hpp"

using namespace Emx;

// OSD配置示例
int main(int argc,char**argv) {
    int code = atoi(argv[1]);
    emxlogd("code:%d\n", code);
    for (int i = 0; i < 2; i++) {
        // 用于存放参数数据
        MediaOsd::Param param = {};
        // 定义一个通道0的编码器对象
        MediaOsd osd(0);
        // 先获取当前的配置
        if (osd.GetParam(param) != ErrCodeE::Success) {
            emxloge("GetParam failed\n");
            return -1;
        }
        if (code == 1) {
            int index = 0;
            // 假设这里需要修改时间戳的对齐方式
            for (auto &item: param.array) {
                if (item->type == MediaOsd::TypeE::Time) {
                    item->ena = true;
                } else if (item->type == MediaOsd::TypeE::Text) {
                    if (index == 0) {
                        item->ena = true;
                        strcpy(item->text.content, "隐私模式");
                        item->margin.rateX = 0;
                        item->margin.rateY = 0;
                        item->margin.horizon = MediaInfOsd::HAlignE::Center;
                        item->margin.vertical = MediaInfOsd::VAlignE::Center;
                    } else {
                        item->ena = false;
                    }
                    index++;
                }
            }
        }

        for (auto &item: param.array) {
            emxlogd("ena:%d, type:%d, content:%s\n", item->ena, item->type, item->text.content);
        }
        if (osd.SetTmpParam(param) != ErrCodeE::Success) {
            emxloge("SetParam failed\n");
            return -1;
        }
        emxlogi("SetParam success\n");
    }
}