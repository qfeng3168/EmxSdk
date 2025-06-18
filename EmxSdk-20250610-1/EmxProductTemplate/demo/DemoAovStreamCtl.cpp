//
// Created by xiong on 2023/8/29.
//

#include "EmxMedia.hpp"

using namespace Emx;

// 异步实时流获取示例
class SaveAovStream {
public:
    // MediaStreamAsync
    SaveAovStream() {}

    void Create(EuvLoop &loop, char *buffer, int size, const char *pathDir) {
        memset(m_pathDir, 0, sizeof(m_pathDir));
        strncpy(m_pathDir, pathDir, sizeof(m_pathDir));
        char name[256] = {};
        sprintf(name, "%s/low_frame_video_%lld.h265", m_pathDir, (long long unsigned int) Time::GetMs64());
        m_file = fopen(name, "wb");

        char name2[256] = {};
        sprintf(name2, "%s/high_frame_video_%lld.h265", m_pathDir, (long long unsigned int) Time::GetMs64());
        m_fileHigh = fopen(name2, "wb");
        // 首先定义通道，并向通道中加入需要监听的媒体帧类型
        MediaStream::Channel channel;
        channel.Add(Emx::MediaFrame::TypeE::Venc, 0);// 加入视频编码通道0
        // 注册当获取到媒体帧时候的回调
        m_stream.Start(loop, buffer, size, channel, std::bind(&SaveAovStream::OnRecvFrame, this, ph_1, ph_2));
        m_msg.Start(loop, m_buffer, (int) sizeof(m_buffer), std::bind(&SaveAovStream::ProcAovMsg, this, ph_1));
    }

    void Destroy() {
        // 停止媒体帧的监听
        m_stream.Stop();
        m_msg.Stop();
        if (m_file) {
            fclose(m_file);
            m_file = nullptr;
        }
        if (m_fileHigh) {
            fclose(m_fileHigh);
            m_fileHigh = nullptr;
        }
    }

private:
    // 当收到媒体帧时触发此回调
    void OnRecvFrame(ErrCodeE e, MediaFrame &frame) {
        // 如果获取失败则返回
        if (e != Emx::ErrCodeE::Success) {
            emxlogw("recv stream failed,type=%d,size=%d\n", frame.type, frame.size);
            return;
        }
        emxlogi("recv stream ,type=%d,size=%d\n", frame.type, frame.size);

        //这里只会收到m_stream.Start的时候在channel中Add的帧类型+通道的帧
        if (frame.type == Emx::MediaFrame::TypeE::Venc) {
            // 收到视频帧
            if (m_isHighFrameRate) {
                fwrite((const char *) frame.data, 1, frame.size, m_fileHigh);
            } else {
                fwrite((const char *) frame.data, 1, frame.size, m_file);
                MediaAov aov(0);
                aov.UpdateStatus((MediaInfAov::AovStatusTypeE::VideoFrameSuccessful));
            }
        }
    }
    void ProcAovMsg(MediaInfAov::AovPushFrame &frame) {
        emxlogi("Receive type: %d,data=%s,size=%d\n", frame.type,frame.data,frame.size);
        switch (frame.type) {
            case MediaInfAov::AovPushTypeE::SleepReady: {
                // 休眠准备,将需要休眠处理的接口休眠掉
                emxlogi("Receive a aov message: SleepReady \n");
                break;
            }
            case MediaInfAov::AovPushTypeE::HighFrameRate: {
                // 进入高帧率模式工作
                m_isHighFrameRate = true;
                emxlogi("Receive a aov message: HighFrameRate \n");
                break;
            }
            case MediaInfAov::AovPushTypeE::LowFrameRate: {
                // 进入低帧率模式工作
                m_isHighFrameRate = false;
                emxlogi("Receive a aov message: LowFrameRate \n");
                break;
            }
            default:
                break;
        }
    }

private:
    MediaStreamAsync m_stream;// 定义一个异步音视频流的监听句柄
    MediaAovMessage m_msg;
    char m_pathDir[128];
    FILE *m_file = nullptr;
    FILE *m_fileHigh = nullptr;
    bool m_isHighFrameRate = false;
    char m_buffer[1024];
};

int main(int argc, char **argv) {
    if (argc < 2) {
        emxloge("usage: %s <pathDir>\n", argv[0]);
        return -1;
    }
    EuvLoop loop;      // 定义一个loop用于所有事件的监听
    SaveAovStream demo;// 定义demo对象
    // loop取名为SaveAovStream，当loop销毁时执行demo.Destroy()
    loop.Init("SaveAovStream", [&]() { demo.Destroy(); }, Emx::EuvLoop::Dynamic);

    //创建
    char buffer[1024];
    demo.Create(loop, buffer, (int) sizeof(buffer), argv[1]);

    // loop启动
    loop.Start();

    printf("input 'q' to exit\n");
    while ('q' != getchar());

    // 此处会出发loop.Init的时候注册的回调，并执行其中的demo.Destroy()
    loop.StopAndDeInit();

    MediaAov aov(0);
    aov.UpdateStatus((MediaInfAov::AovStatusTypeE::VideoFrameFailed));
    printf("exit success\n");
    return 0;
}