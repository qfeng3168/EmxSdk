//
// Created by xiong on 2023/8/29.
//
#include "EmxMedia.hpp"

using namespace Emx;

// 缩略图抓取示例
int main() {
    // 抓拍一张图片
    // 添加需要监听的Snap通道，这里监听snap通道0
    MediaStream::Channel channel;
    channel.Add(MediaFrame::TypeE::Snap, 0);

    // 启动snap的监听
    MediaStreamSync stream;
    ErrCodeE e = stream.Open(channel, 3000);
    if (e != ErrCodeE::Success) {
        emxloge("open snap failed\n");
        return -1;
    }

    // 触发一个snap 通道0的信号，MediaServer收到这个信号后会立刻发送一帧缩略图
    MediaSnap snap(0);
    snap.Trigger(3000);

    // 接收这帧缩略图
    MediaFrame frame = {};
    e = stream.GetFrame(frame, 3000);
    if (e != ErrCodeE::Success) {
        emxloge("get snap failed\n");
        return -1;
    }

    // 将缩略图保存下来
    FILE *fp = fopen("./snap.jpg", "wb+");
    if (!fp) {
        emxloge("open snap failed\n");
        stream.Close();
        return -1;
    }
    fwrite(frame.data, 1, frame.size, fp);

    // 关闭snap接收通道
    stream.Close();
    return 0;
}