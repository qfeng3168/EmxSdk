//
// Created by xiong on 2023/8/29.
//
#include "EmxMedia.hpp"
#include <thread>

using namespace Emx;

// 实时流解码示例
class VdecSyncStream {
public:
    VdecSyncStream() { }

    void Create(EuvLoop &loop, char *buffer, int size) {
        // 首先定义通道，并向通道中加入需要监听的媒体帧类型
        MediaStream::Channel channel;
        channel.Add(Emx::MediaFrame::TypeE::Venc, 0); // 加入视频编码通道0
        channel.Add(Emx::MediaFrame::TypeE::Aenc, 0); // 加入音频编码通道0
        // 注册当获取到媒体帧时候的回调
        m_stream.Start(loop, buffer, size, channel, std::bind(&VdecSyncStream::OnRecvFrame, this, ph_1, ph_2));

        m_vdecStream = new MediaVdecStream(0);
        m_vdecStream->Create();
    }

    void Destroy() {
        // 停止媒体帧的监听
        m_stream.Stop();
        m_vdecStream->Destroy();
        delete(m_vdecStream);
    }

private:
    // 当收到媒体帧时触发此回调
    void OnRecvFrame(ErrCodeE e, MediaFrame &frame) {
        // 如果获取失败则返回
        if (e != Emx::ErrCodeE::Success) {
            emxlogw("recv stream failed\n");
            return;
        }

        //这里只会收到m_stream.Start的时候在channel中Add的帧类型+通道的帧
        if (frame.type == Emx::MediaFrame::TypeE::Venc) {
            // 收到视频帧
            MediaInfVdec::FrameData frameOut = {};
            frameOut.tsInUs = frame.tsInUs;
            frameOut.data.data = (uint8_t *) frame.data;
            frameOut.data.size = (int) frame.size;
            m_vdecStream->PushFrame(frameOut,1000);
        }
    }

private:
    MediaVdecStream *m_vdecStream;
    MediaStreamAsync m_stream; // 定义一个异步音视频流的监听句柄
};

int main() {
    EuvLoop loop; // 定义一个loop用于所有事件的监听
    VdecSyncStream demo; // 定义demo对象
    // loop取名为VdecSyncStream，当loop销毁时执行demo.Destroy()
    loop.Init("VdecSyncStream", [&]() { demo.Destroy(); }, Emx::EuvLoop::Dynamic);

    //创建
    char buffer[1024];
    demo.Create(loop, buffer, (int) sizeof(buffer));

    // loop启动
    loop.Start();

    printf("input 'q' to exit\n");
    while ('q' != getchar());

    // 此处会出发loop.Init的时候注册的回调，并执行其中的demo.Destroy()
    loop.StopAndDeInit();

    printf("exit success\n");
    return 0;
}
