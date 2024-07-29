//
// Created by xiong on 2023/8/29.
//

#include "EmxMedia.hpp"

using namespace Emx;

// 异步实时流获取示例
class SaveAsyncStream {
public:
    // MediaStreamAsync
    SaveAsyncStream() {
        m_fpVenc = nullptr;
        m_fpAenc = nullptr;
    }

    void Create(EuvLoop &loop, char *buffer, int size) {
        // 首先定义通道，并向通道中加入需要监听的媒体帧类型
        MediaStream::Channel channel;
        channel.Add(Emx::MediaFrame::TypeE::Venc, 0); // 加入视频编码通道0
        channel.Add(Emx::MediaFrame::TypeE::Aenc, 0); // 加入音频编码通道0
        // 注册当获取到媒体帧时候的回调
        m_stream.Start(loop, buffer, size, channel, std::bind(&SaveAsyncStream::OnRecvFrame, this, ph_1, ph_2));
        // 创建两个文件用来保存音频和视频的裸数据
        m_fpVenc = fopen("video.raw", "wb+");
        m_fpAenc = fopen("audio.raw", "wb+");
    }

    void Destroy() {
        // 停止媒体帧的监听
        m_stream.Stop();
        // 关闭文件
        if (m_fpVenc) {
            fclose(m_fpVenc);
            m_fpVenc = nullptr;
        }
        if (m_fpAenc) {
            fclose(m_fpAenc);
            m_fpAenc = nullptr;
        }
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
            fwrite(frame.data, 1, frame.size, m_fpVenc);
        } else {
            // 收到音频帧
            fwrite(frame.data, 1, frame.size, m_fpAenc);
        }
    }

private:
    MediaStreamAsync m_stream; // 定义一个异步音视频流的监听句柄
    FILE *m_fpVenc; // 定义file指针用来存储编码后的视频裸流
    FILE *m_fpAenc; // 定义file指针用来存储编码后的音频裸流
};

int main() {
    EuvLoop loop; // 定义一个loop用于所有事件的监听
    SaveAsyncStream demo; // 定义demo对象
    // loop取名为SaveAsyncStream，当loop销毁时执行demo.Destroy()
    loop.Init("SaveAsyncStream", [&]() { demo.Destroy(); }, Emx::EuvLoop::Dynamic);

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