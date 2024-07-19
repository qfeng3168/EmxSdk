//
// Created by xiong on 2023/8/29.
//

#include "EmxMedia.hpp"

using namespace Emx;

int test1() {
    MediaVenc venc(0);
    MediaOsd osd(0);
    MediaVenc::Param param;

    venc.GetParam(param);
    emxlogd("src width:%d, height:%d\n", param.width, param.height);
    if (param.width == 2880 && param.height == 1620) {
        param.width = 1920;
        param.height = 1080;
    } else if (param.width == 1920 && param.height == 1080) {
        param.width = 2880;
        param.height = 1620;
    }
    emxlogd("dst width:%d, height:%d\n", param.width, param.height);
    venc.SetParam(param);

    MediaOsd::Param defOsdPms = {};
    if (osd.GetParam(defOsdPms) != ErrCodeE::Success) { emxloge("get osd info failed!\n"); }

    int fontsize = 0;
    if (param.height == 1080) {
        fontsize = 48;
    } else {
        fontsize = 70;
    }
    emxlogi("fontsize:%d\n", fontsize);
    for (auto &item: defOsdPms.array) {
        if (item->type == MediaOsd::TypeE::Time || item->type == MediaOsd::TypeE::Text) { item->text.size = fontsize; }
    }
    if (osd.SetParam(defOsdPms) != ErrCodeE::Success) { emxloge("set osd info failed!\n"); }
    return 0;
}
int test2() {
    MediaVenc venc(1);
    MediaOsd osd(1);
    MediaVenc::Param param;

    venc.GetParam(param);
    emxlogd("src width:%d, height:%d\n", param.width, param.height);
    if (param.width == 1280 && param.height == 720) {
        param.width = 960;
        param.height = 540;
    } else if (param.width == 960 && param.height == 540) {
        param.width = 1280;
        param.height = 720;
    }
    emxlogd("dst width:%d, height:%d\n", param.width, param.height);
    venc.SetParam(param);

    MediaOsd::Param defOsdPms = {};
    if (osd.GetParam(defOsdPms) != ErrCodeE::Success) { emxloge("get osd info failed!\n"); }

    int fontsize = 0;
    if (param.height == 540) {
        fontsize = 20;
    } else {
        fontsize = 32;
    }
    emxlogi("fontsize:%d\n", fontsize);
    for (auto &item: defOsdPms.array) {
        if (item->type == MediaOsd::TypeE::Time || item->type == MediaOsd::TypeE::Text) { item->text.size = fontsize; }
    }
    if (osd.SetParam(defOsdPms) != ErrCodeE::Success) { emxloge("set osd info failed!\n"); }
    return 0;
}
void test3() {
    int entryNum = 0;
    int leaveNum = 0;
    while (1) {
        // 处理客流消息
        MediaOsd osd(0);
        MediaOsd::Param defOsdPms = {};
        if (osd.GetParam(defOsdPms) != ErrCodeE::Success) { emxloge("get osd info failed!\n"); }
        auto item = std::make_shared<MediaOsd::ParamItem>();
        item->ena = true;
        item->type = MediaInfOsd::TypeE::Text;
        item->layer = 1;
        item->margin.horizon = MediaInfOsd::HAlignE::Left;
        item->margin.vertical = MediaInfOsd::VAlignE::Top;
        item->margin.rateX = 7000;
        item->margin.rateY = 800;
        item->text.size = 56;
        item->text.thickness = 1;
        item->text.wrapLine = false;
        item->text.lineSpace = 100;
        strcpy(item->text.font, "font.ttf");
        sprintf(item->text.content, "进入: %d,离开: %d", entryNum++, leaveNum++);
        defOsdPms.array.push_back(item);
        int ret = (int)osd.SetTmpParam(defOsdPms);
        emxlogi("Set osd Param ret=%d\n",ret); 
        usleep(100000);
    }
}
// OSD配置示例
int main(int argc, char **argv) {
    // test1();
    // test2();
    test3();
    return 0;
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