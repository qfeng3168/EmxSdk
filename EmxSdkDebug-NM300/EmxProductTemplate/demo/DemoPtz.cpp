//
// Created by xiong on 2023/8/29.
//
#include "EmxModules.hpp"

using namespace Emx;

// Ptz使用示例
int main(){
    // 定义一个Ptz对象
    Ptz ptz;
    // 初始化ptz，这里会导致阻塞，等待ptz复位完成才会返回
    auto e = ptz.Init();
    if (e != Emx::ErrCodeE::Success) {
        emxloge("init ptz failed\n");
        return -1;
    }
    // 获取ptz水平电机当前的状态，使用阻塞方式
    Ptz::Status status = {};
    e = ptz.GetStat(Emx::Ptz::TypeE::Horizon, true, status);
    if (e != Emx::ErrCodeE::Success) {
        emxloge("ptz Horizon GetStat failed\n");
        return -1;
    }
    // 获取ptz垂直电机当前的状态，使用非阻塞方式，有可能会获取失败
    e = ptz.GetStat(Emx::Ptz::TypeE::Vertical, false, status);
    if (e != Emx::ErrCodeE::Success) {
        emxloge("ptz Vertical GetStat failed\n");
    }
    // 移动的步数和速度可参考之前status获取到的step、speed的min、max、current
    // 使用阻塞方式，使用默认速度，向正方向移动100步
    ptz.MoveStep(Emx::Ptz::TypeE::Horizon, true, 100);
    // 使用非阻塞方式，使用默认速度，向反方向移动500步
    ptz.MoveStep(Emx::Ptz::TypeE::Horizon, false, -500);
    bool busy;
    do {
        // 获取水平电机是否正在运行，可用于应用过滤移动侦测事件告警等
        busy = ptz.IsBusy(Emx::Ptz::TypeE::Horizon);
        emxlogd("ptz Horizon Busy = %d\n", busy);
        usleep(100000);
    } while (busy);
    // 销毁ptz对象
    ptz.DeInit();
}