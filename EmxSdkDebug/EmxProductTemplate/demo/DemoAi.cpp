//
// Created by xiong on 2023/8/29.
//
#include "EmxMedia.hpp"

using namespace Emx;

// 异步AI消息获取示例，调用方式与获取实时流类似
class Ai {
public:
    void Create(EuvLoop &loop) {
        // AI的获取不需要配置监听类型和通道，默认监听所有通道
        m_ai.Start(&loop, m_buffer, (int) sizeof(m_buffer), std::bind(&Ai::ProcAiData, this, ph_1));
    }

    void Destroy() {
        // 销毁
        m_ai.Stop();
    }

private:
    void ProcAiData(MediaInfAiData::Header &header) {
        emxlogd("header type: %d\n", header.type);
        switch (header.type) {
            case MediaInfAiData::TypeE::MotionDetection: {
                // 处理移动侦测消息
                MediaInfAiDataMotionDetection data;
                if (m_ai.GetDataByInfo(header, &data) != ErrCodeE::Success)
                    return;
                // 遍历所有数据，例如：如果检测到运动面积大于10%，就认为是移动事件
                // 另外基于data中的x/y/w/h以及header中的srcPicSize，可以用来定位事件在画面中的位置
                // 可以用来做移动追踪或者检测区域的过滤
                emxlogd("data num: %d\n", data.num);
                for (int i = 0; i < data.num; i++) {
                    if (data.array[i].Area() > header.srcPicSize.w * header.srcPicSize.h * 10 / 100) {
                        emxlogi("motion alarm triggered\n");
                        return;
                    }
                }
                return;
            }
            case MediaInfAiData::TypeE::PersonDetection: {
                // 处理人行侦测消息
                MediaInfAiDataPersonDetection data;
                if (m_ai.GetDataByInfo(header, &data) != ErrCodeE::Success)
                    return;
                // 遍历所有数据，例如：如果检测到运动面积大于10%，就认为是人形事件
                // 另外基于data中的x/y/w/h以及header中的srcPicSize，可以用来定位事件在画面中的位置
                // 可以用来做移动追踪或者检测区域的过滤
                emxlogd("data num: %d\n", data.num);
                for (int i = 0; i < data.num; i++) {
                    if (data.array[i].Area() > header.srcPicSize.w * header.srcPicSize.h * 10 / 100) {
                        emxlogi("person alarm triggered\n");
                        return;
                    }
                }
                return;
            }
            case MediaInfAiData::TypeE::FaceSnap:
                break;
            default:
                break;
        }
    }

private:
    MediaAiDataRecvAsync m_ai;
    char m_buffer[256];
};

int main() {
    EuvLoop loop; // 定义一个loop用于所有事件的监听
    Ai demo = {}; // 定义demo对象
    // loop取名为Ai，当loop销毁时执行demo.Destroy()
    loop.Init("Ai", [&]() { demo.Destroy(); }, Emx::EuvLoop::Dynamic);

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