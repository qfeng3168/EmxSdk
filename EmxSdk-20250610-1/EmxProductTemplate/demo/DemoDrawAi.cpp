//
// Created by xiong on 2023/8/29.
//
#include "EmxMedia.hpp"
#include "media/client/MediaDraw.hpp"

using namespace Emx;

// 异步AI消息获取示例，调用方式与获取实时流类似
class Ai {
public:
    void Create(EuvLoop &loop,char *dir) {
        // AI的获取不需要配置监听类型和通道，默认监听所有通道
        m_ai.Start(&loop, m_buffer, (int) sizeof(m_buffer), std::bind(&Ai::ProcAiData, this, ph_1));

        if(dir){
            isRecored = true;
            strcpy(imgDir,dir);
            if(access(imgDir,F_OK)){
                mkdir(imgDir,0777);
            }
        }
    }

    void Destroy() {
        // 销毁
        m_ai.Stop();
    }

private:
    void ProcAiData(MediaInfAiData::Header &header) {
        emxlogd("header type: %d,chn=%d,size=%d\n", header.type,header.chn, header.size);
        MediaInfDraw::Param drawDefPms = {};
        MediaDraw draw(header.chn==0?0:2);
        switch (header.type) {
            case MediaInfAiData::TypeE::MotionDetection: {
                // 处理移动侦测消息
                MediaInfAiDataMotionDetection data;
                if (m_ai.GetDataByInfo(header, &data) != ErrCodeE::Success) break;
                // 遍历所有数据，例如：如果检测到运动面积大于10%，就认为是移动事件
                // 另外基于data中的x/y/w/h以及header中的srcPicSize，可以用来定位事件在画面中的位置
                // 可以用来做移动追踪或者检测区域的过滤
                emxlogd("motion data num: %d\n", data.num);
                for (int i = 0; i < data.num; i++) {
                    emxlogi("[%d]x=%d,y=%d,w=%d,h=%d\n", i, data.array[i].x, data.array[i].y, data.array[i].w,
                            data.array[i].h);
                    std::shared_ptr<MediaInfDraw::ParamItem> item = std::make_shared<MediaInfDraw::ParamItem>();
                    item->ena = true;
                    item->color = 0xff00FFff;
                    item->thickness = 2;
                    item->type = MediaInfDraw::TypeE::Rect;
                    item->rect.fill = 0;
                    item->rect.x = data.array[i].x * 10000 / header.srcPicSize.w;
                    item->rect.y = data.array[i].y * 10000 / header.srcPicSize.h;
                    item->rect.width = data.array[i].w * 10000 / header.srcPicSize.w;
                    item->rect.height = data.array[i].h * 10000 / header.srcPicSize.h;
                    drawDefPms.array.emplace_back(item);
                }
                break;
            }
            case MediaInfAiData::TypeE::PersonDetection: {
                // 处理人行侦测消息
                MediaInfAiDataPersonDetection data;
                if (m_ai.GetDataByInfo(header, &data) != ErrCodeE::Success) break;
                // 遍历所有数据，例如：如果检测到运动面积大于10%，就认为是人形事件
                // 另外基于data中的x/y/w/h以及header中的srcPicSize，可以用来定位事件在画面中的位置
                // 可以用来做移动追踪或者检测区域的过滤
                emxlogd("person data num: %d\n", data.num);
                for (int i = 0; i < data.num; i++) {
                    emxlogi("[%d]x=%d,y=%d,w=%d,h=%d\n", i, data.array[i].x, data.array[i].y, data.array[i].w,
                            data.array[i].h);
                    std::shared_ptr<MediaInfDraw::ParamItem> item = std::make_shared<MediaInfDraw::ParamItem>();
                    item->ena = true;
                    item->color = 0xff0000ff;
                    item->thickness = 2;
                    item->type = MediaInfDraw::TypeE::Rect;
                    item->rect.fill = 0;
                    item->rect.x = data.array[i].x * 10000 / header.srcPicSize.w;
                    item->rect.y = data.array[i].y * 10000 / header.srcPicSize.h;
                    item->rect.width = data.array[i].w * 10000 / header.srcPicSize.w;
                    item->rect.height = data.array[i].h * 10000 / header.srcPicSize.h;
                    drawDefPms.array.emplace_back(item);
                }
                break;
            }
            case MediaInfAiData::TypeE::InvasionDetection: {
                // 处理入侵侦测消息
                MediaInfAiDataInvasionDetection data;
                if (m_ai.GetDataByInfo(header, &data) != ErrCodeE::Success) break;
                emxlogd("invasion data num: %d\n", data.num);
                for (int i = 0; i < data.num; i++) {
                    emxlogi("[%d]id=%lld,x=%d,y=%d,w=%d,h=%d\n", i, data.array[i].id, data.array[i].rect.x,
                            data.array[i].rect.y, data.array[i].rect.w, data.array[i].rect.h);
                }
                break;
            }
            case MediaInfAiData::TypeE::CrossingDetection: {
                // 处理越界侦测消息
                MediaInfAiDataInvasionDetection data;
                if (m_ai.GetDataByInfo(header, &data) != ErrCodeE::Success) break;
                emxlogd("cross data num: %d\n", data.num);
                for (int i = 0; i < data.num; i++) {
                    emxlogi("[%d]id=%lld,x=%d,y=%d,w=%d,h=%d\n", i, data.array[i].id, data.array[i].rect.x,
                            data.array[i].rect.y, data.array[i].rect.w, data.array[i].rect.h);
                }
                break;
            }
            case MediaInfAiData::TypeE::PassengerFlowStatistics: {
                // 处理客流消息
                MediaInfAiDataPassengerFlowStatistics data;
                if (m_ai.GetDataByInfo(header, &data) != ErrCodeE::Success) break;
                emxlogd("kltj data entryNum: %d,leaveNum: %d\n", data.msg.entryNum, data.msg.leaveNum);
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
                item->text.wrapLine = true;
                item->text.lineSpace = 100;
                strcpy(item->text.font, "font.ttf");
                sprintf(item->text.content, "进入: %d\\n离开: %d", data.msg.entryNum, data.msg.leaveNum);

                defOsdPms.array.push_back(item);

                if (osd.SetTmpParam(defOsdPms) != ErrCodeE::Success) { emxloge("Set osd Param failed\n"); }
                break;
            }
            case MediaInfAiData::TypeE::FaceDetection: {
                // 处理人脸侦测消息
                MediaInfAiDataFaceDetection data;
                if (m_ai.GetDataByInfo(header, &data) != ErrCodeE::Success) break;
                emxlogd("face detection data num: %d\n", data.num);
                for (int i = 0; i < data.num; i++) {
                    emxlogi("[%d]x=%d,y=%d,w=%d,h=%d\n", i, data.array[i].x, data.array[i].y, data.array[i].w,
                            data.array[i].h);
                    std::shared_ptr<MediaInfDraw::ParamItem> item = std::make_shared<MediaInfDraw::ParamItem>();
                    item->ena = true;
                    item->color = 0xff00FFff;
                    item->thickness = 2;
                    item->type = MediaInfDraw::TypeE::Rect;
                    item->rect.fill = 0;
                    item->rect.x = data.array[i].x * 10000 / header.srcPicSize.w;
                    item->rect.y = data.array[i].y * 10000 / header.srcPicSize.h;
                    item->rect.width = data.array[i].w * 10000 / header.srcPicSize.w;
                    item->rect.height = data.array[i].h * 10000 / header.srcPicSize.h;
                    drawDefPms.array.emplace_back(item);
                }
                break;
            }
            case MediaInfAiData::TypeE::FaceRecognition: {
                // 处理人脸消息
                MediaInfAiDataFaceRecognition data;
                if (m_ai.GetDataByInfo(header, &data) != ErrCodeE::Success) return;
                emxlogd("face recognition data num: %d\n", data.num);
                if (isRecored && data.fullPic.size > 0 && data.fullPic.img) {
                    std::string fileName = imgDir;
                    fileName += "face_full";
                    fileName += std::to_string(Time::GetMs());
                    fileName += ".jpg";
                    FILE *fp = fopen(fileName.c_str(), "wb+");
                    if (fp) {
                        fwrite(data.fullPic.img, 1, data.fullPic.size, fp);
                        fclose(fp);
                    }
                }
                for (int i = 0; i < data.num; i++) {
                    emxlogi("[%d]id=%lld,x=%d,y=%d,w=%d,h=%d\n", i, data.array[i].id, data.array[i].rect.x,
                            data.array[i].rect.y, data.array[i].rect.w, data.array[i].rect.h);
                    std::shared_ptr<MediaInfDraw::ParamItem> item = std::make_shared<MediaInfDraw::ParamItem>();
                    item->ena = true;
                    item->color = 0x00FF00ff;
                    item->thickness = 2;
                    item->type = MediaInfDraw::TypeE::Rect;
                    item->rect.fill = 0;
                    item->rect.x = data.array[i].rect.x * 10000 / header.srcPicSize.w;
                    item->rect.y = data.array[i].rect.y * 10000 / header.srcPicSize.h;
                    item->rect.width = data.array[i].rect.w * 10000 / header.srcPicSize.w;
                    item->rect.height = data.array[i].rect.h * 10000 / header.srcPicSize.h;
                    drawDefPms.array.emplace_back(item);

                    if (isRecored && data.array[i].pic.size > 0 && data.array[i].pic.img) {
                        std::string fileName = imgDir;
                        fileName += "face_";
                        fileName += std::to_string(Time::GetMs());
                        fileName += "_";
                        fileName += std::to_string(i);
                        fileName += ".jpg";
                        FILE *fp = fopen(fileName.c_str(), "wb+");
                        if (fp) {
                            fwrite(data.array[i].pic.img, 1, data.array[i].pic.size, fp);
                            fclose(fp);
                        }
                    }
                }
                break;
            }
            case MediaInfAiData::TypeE::VehicleRecognition: {
                // 处理车辆车牌消息
                MediaInfAiDataVehicleRecognition data;
                if (m_ai.GetDataByInfo(header, &data) != ErrCodeE::Success) return;
                emxlogd("Vehicle recognition data num: %d\n", data.num);
                if (isRecored && data.fullPic.size > 0 && data.fullPic.img) {
                    std::string fileName = imgDir;
                    fileName += "rcf_full";
                    fileName += std::to_string(Time::GetMs());
                    fileName += ".jpg";
                    FILE *fp = fopen(fileName.c_str(), "wb+");
                    if (fp) {
                        fwrite(data.fullPic.img, 1, data.fullPic.size, fp);
                        fclose(fp);
                    }
                }
                for (int i = 0; i < data.num; i++) {
                    emxlogi("[%d]id=%lld,plateId=%lld,x=%d,y=%d,w=%d,h=%d\n", i, data.array[i].id,
                            data.array[i].plateId, data.array[i].rect.x, data.array[i].rect.y, data.array[i].rect.w,
                            data.array[i].rect.h);
                    {
                        std::shared_ptr<MediaInfDraw::ParamItem> item = std::make_shared<MediaInfDraw::ParamItem>();
                        item->ena = true;
                        item->color = 0x88FF00ff;
                        item->thickness = 2;
                        item->type = MediaInfDraw::TypeE::Rect;
                        item->rect.fill = 0;
                        item->rect.x = data.array[i].rect.x * 10000 / header.srcPicSize.w;
                        item->rect.y = data.array[i].rect.y * 10000 / header.srcPicSize.h;
                        item->rect.width = data.array[i].rect.w * 10000 / header.srcPicSize.w;
                        item->rect.height = data.array[i].rect.h * 10000 / header.srcPicSize.h;
                        drawDefPms.array.emplace_back(item);
                    }

                    if (isRecored && data.array[i].pic.size > 0 && data.array[i].pic.img) {
                        std::string fileName = imgDir;
                        fileName += "Vehicle_";
                        fileName += std::to_string(Time::GetMs());
                        fileName += "_";
                        fileName += std::to_string(i);
                        fileName += ".jpg";
                        FILE *fp = fopen(fileName.c_str(), "wb+");
                        if (fp) {
                            fwrite(data.array[i].pic.img, 1, data.array[i].pic.size, fp);
                            fclose(fp);
                        }
                    }
                    if (isRecored && data.array[i].platePic.size > 0 && data.array[i].platePic.img) {
                         {
                            std::shared_ptr<MediaInfDraw::ParamItem> item = std::make_shared<MediaInfDraw::ParamItem>();
                            item->ena = true;
                            item->color = 0x00FF88ff;
                            item->thickness = 2;
                            item->type = MediaInfDraw::TypeE::Rect;
                            item->rect.fill = 0;
                            item->rect.x = data.array[i].plateRect.x * 10000 / header.srcPicSize.w;
                            item->rect.y = data.array[i].plateRect.y * 10000 / header.srcPicSize.h;
                            item->rect.width = data.array[i].plateRect.w * 10000 / header.srcPicSize.w;
                            item->rect.height = data.array[i].plateRect.h * 10000 / header.srcPicSize.h;
                            drawDefPms.array.emplace_back(item);
                        }
                        std::string fileName = imgDir;
                        fileName += "plate_";
                        fileName += std::to_string(Time::GetMs());
                        fileName += "_";
                        fileName += std::to_string(i);
                        fileName += ".jpg";
                        FILE *fp = fopen(fileName.c_str(), "wb+");
                        if (fp) {
                            fwrite(data.array[i].platePic.img, 1, data.array[i].platePic.size, fp);
                            fclose(fp);
                        }
                    }
                }
                break;
            }
            case MediaInfAiData::TypeE::PersonRecognition: {
                // 处理人侦测消息
                MediaInfAiDataPersonRecognition data;
                if (m_ai.GetDataByInfo(header, &data) != ErrCodeE::Success) break;
                emxlogd("MediaInfAiDataPersonRecognition data num: %d\n", data.num);
                for (int i = 0; i < data.num; i++) {
                    emxlogi("[%d]x=%d,y=%d,w=%d,h=%d\n", i, data.array[i].rect.x, data.array[i].rect.y, data.array[i].rect.w,
                            data.array[i].rect.h);
                    std::shared_ptr<MediaInfDraw::ParamItem> item = std::make_shared<MediaInfDraw::ParamItem>();
                    item->ena = true;
                    item->color = 0xffff00ff;
                    item->thickness = 2;
                    item->type = MediaInfDraw::TypeE::Rect;
                    item->rect.fill = 0;
                    item->rect.x = data.array[i].rect.x * 10000 / header.srcPicSize.w;
                    item->rect.y = data.array[i].rect.y * 10000 / header.srcPicSize.h;
                    item->rect.width = data.array[i].rect.w * 10000 / header.srcPicSize.w;
                    item->rect.height = data.array[i].rect.h * 10000 / header.srcPicSize.h;
                    drawDefPms.array.emplace_back(item);
                }
                if(isRecored && data.fullPic.img!=nullptr && data.fullPic.size>0){
                    std::string fileName = imgDir;
                    fileName += "Person_";
                    fileName += std::to_string(Time::GetMs());
                    fileName += ".jpg";
                    FILE *fp = fopen(fileName.c_str(), "wb+");
                    if (fp) {
                        fwrite(data.fullPic.img, 1, data.fullPic.size, fp);
                        fclose(fp);
                    }
                }
                break;
            }
            default:
                break;
        }
        if (draw.SetTmpParam(drawDefPms) != ErrCodeE::Success) { emxloge("set draw info failed!\n"); }
    }

private:
    MediaAiDataRecvAsync m_ai;
    char m_buffer[256];
    char imgDir[128];
    bool isRecored = false;
};

int main(int argc,char **argv) {
    EuvLoop loop;// 定义一个loop用于所有事件的监听
    Ai demo = {};// 定义demo对象
    // loop取名为Ai，当loop销毁时执行demo.Destroy()
    loop.Init("Ai", [&]() { demo.Destroy(); }, Emx::EuvLoop::Dynamic);

    //创建
    demo.Create(loop,argv[1]);

    // loop启动
    loop.Start();

    printf("input 'q' to exit\n");
    while ('q' != getchar());

    // 此处会出发loop.Init的时候注册的回调，并执行其中的demo.Destroy()
    loop.StopAndDeInit();

    printf("exit success\n");
    return 0;
}