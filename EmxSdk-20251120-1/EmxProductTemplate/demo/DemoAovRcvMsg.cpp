//
// Created by tangfahong on 2025/6/10.
//
#include "EmxMedia.hpp"

using namespace Emx;

// 异步Aov消息获取示例
class Aov {
public:
    void Create(EuvLoop &loop) {
        // AI的获取不需要配置监听类型和通道，默认监听所有通道
        m_aov.Start(loop, m_buffer, (int) sizeof(m_buffer), std::bind(&Aov::ProcAovMsg, this, ph_1));
    }

    void Destroy() {
        // 销毁
        m_aov.Stop();
    }

private:
    void ProcAovMsg(MediaInfAov::AovPushFrame &frame) {
        switch (frame.type) {
            case MediaInfAov::AovPushTypeE::SleepReady: {
                // 休眠准备,将需要休眠处理的接口休眠掉
                emxlogi("Receive a aov message: SleepReady \n");
                break;
            }
            case MediaInfAov::AovPushTypeE::HighFrameRate: {
                // 进入高帧率模式工作
                emxlogi("Receive a aov message: HighFrameRate \n");
                break;
            }
            default:
                break;
        }
    }

private:
    MediaAovMessage m_aov;
    char m_buffer[256];
};


int main() {
    EuvLoop loop; // 定义一个loop用于所有事件的监听
    Aov demo = {}; // 定义demo对象
    // loop取名为Aov，当loop销毁时执行demo.Destroy()
    loop.Init("AovDemo", [&]() { demo.Destroy(); }, Emx::EuvLoop::Dynamic);

    //创建
    demo.Create(loop);

    // loop启动
    loop.Start();

    printf("input 'q' to exit\n");
    while ('q' != getchar());

    // 此处会出发loop.Init的时候注册的回调，并执行其中的demo.Destroy()
    loop.StopAndDeInit();

    printf("exit success\n");
    return 0;
}