//
// Created by xiong on 2023/8/29.
//
#include "EmxMedia.hpp"
#include <thread>

using namespace Emx;

// 同步实时流获取示例
class SaveSyncStream {
public:
    void Create() {
        m_quit = false;
        m_thread = std::thread([this]() {
            FILE *fpVenc = fopen("video.raw", "wb+");// 定义file指针用来存储编码后的视频裸流
            FILE *fpAenc = fopen("audio.raw", "wb+");// 定义file指针用来存储编码后的音频裸流
            // 首先定义通道，并向通道中加入需要监听的媒体帧类型
            MediaStream::Channel channel;
            channel.Add(Emx::MediaFrame::TypeE::Venc, 0); // 加入视频编码通道0
            channel.Add(Emx::MediaFrame::TypeE::Aenc, 0); // 加入音频编码通道0
            MediaStreamSync stream;
            stream.Open(channel, 1000);
            while (!m_quit) {
                MediaFrame frame = {};
                auto e = stream.GetFrame(frame, 1000);
                if (e != Emx::ErrCodeE::Success)
                    continue;
                //这里只会收到m_stream.Start的时候在channel中Add的帧类型+通道的帧
                if (frame.type == Emx::MediaFrame::TypeE::Venc) {
                    // 收到视频帧
                    fwrite(frame.data, 1, frame.size, fpVenc);
                } else {
                    // 收到音频帧
                    fwrite(frame.data, 1, frame.size, fpAenc);
                }
            }
            stream.Close();
            fclose(fpVenc);
            fclose(fpAenc);
        });
    }

    void Destroy() {
        m_quit = true;
        m_thread.join();
    }

private:
    std::thread m_thread;
    bool m_quit;
};

int main() {
    SaveSyncStream demo; // 定义demo对象
    //创建
    demo.Create();
    printf("input 'q' to exit\n");
    while ('q' != getchar());
    demo.Destroy();
    printf("exit success\n");
    return 0;
}