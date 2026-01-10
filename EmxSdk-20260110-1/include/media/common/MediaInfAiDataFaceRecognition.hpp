//
// Created by tangfahong on 2024/4/16.
//

#ifndef EMX_MediaInfAiDataFaceRecognition_HPP
#define EMX_MediaInfAiDataFaceRecognition_HPP

#include "MediaInfAiData.hpp"
#include "MediaRegion.hpp"
#include <cstring>

namespace Emx {
    /** @addtogroup media-common
     *  @{
     */

    class MediaInfAiDataFaceRecognition : public MediaInfAiData {
    public:
        MediaInfAiDataFaceRecognition() : MediaInfAiData(), num(0), array(nullptr) {
            memset(&fullPic, 0, sizeof(MediaInfAiPic));
        }

        virtual ~MediaInfAiDataFaceRecognition() { Free(); }

    public:
        struct DataMsg {
            MediaInfAiPic pic;     //!<抠图数据
            uint64_t id;           //!<当前抓拍的人脸id
            uint8_t quality;       //!<当前人脸质量[0~100]
            uint8_t sharpness;     //!<当前人脸清晰度[0~100]
            uint8_t age;           //!<年龄
            uint8_t gender;        //!<性别 0=未知, 1=男性, 2=女性
            uint8_t skin;          //!<肤色 0=未知, 1=白, 2=黑,3=黄
            uint8_t glasses;       //!<眼镜 0=未知, 1=佩戴,2=未佩戴
            uint8_t beard;         //!<胡须 0=未知, 1=有,2=没有
            uint8_t hat;           //!<帽子 0=未知, 1=佩戴,2=未佩戴
            uint8_t hairColor;     //!<头发颜色 0=未知,1=黑,2=黄,3=红,4=棕,5=绿,6=蓝,7=紫,8=白,9=粉色
            uint8_t hairStyle;     //!<头发发型 0=未知,1=短发,2=长发,3=直发,4=卷发,5=马尾,6=辫子,7=盘发
            uint8_t emotion;       //!<情绪 0=未知,1=高兴,2=惊讶,3=恐惧,4=厌恶,5=难过,6=愤怒,7=平静
            uint8_t liveness;      //!<活体 0=未知,1=是,2=不是
            int32_t ipd;           //!<人脸瞳距
            uint64_t captureTime;  //!<人脸抓拍到的毫秒时间戳
            uint64_t startTime;    //!<人脸开始出现时间的毫秒时间戳
            uint64_t endTime;      //!<人脸结束时间,毫秒时间戳
            Rect rect;             //!<当前人脸位置信息
            uint32_t isMask;       //!<是否佩戴口罩
            uint32_t maskColor;    //!<口罩颜色
            uint32_t maskType;     //!<口罩类型
            int32_t libId;         //!<人脸库id 如果有人脸识别就返回相似度最高的人脸库id
            int32_t similarity;    //!<人脸和人脸库id相似度 [0~100]
            KeyPoint keyPoints[32];//!<人脸关键点
            int keyPointNums;      //!<人脸关键点数量
            int imgOff;            //!<私有数据，记录内存偏移
        };

        void Alloc(int n) {
            Free();
            array = (DataMsg *) EP_MALLOC(n * (int) sizeof(DataMsg));
            if (array) { memset((void *) array, 0, n * sizeof(DataMsg)); }
            memset(&fullPic, 0, sizeof(MediaInfAiPic));
        }

        void Free() {
            if (array) {
                for (int i = 0; i < num; i++) {
                    if ((array[i].pic.size > 0) && array[i].pic.img) {
                        free(array[i].pic.img);
                        array[i].pic.img = nullptr;
                        array[i].pic.size = 0;
                    }
                }
                if ((fullPic.size > 0) && fullPic.img) {
                    free(fullPic.img);
                    fullPic.img = nullptr;
                }
                free(array);
                array = nullptr;
            }
        }

        int GetSerializeSize() override {
            int totalImgSize = 0;
            totalImgSize += sizeof(MediaInfAiPic);
            totalImgSize += fullPic.size;
            for (int i = 0; i < num; i++) { totalImgSize += array[i].pic.size; }
            return (int) sizeof(num) + num * (int) sizeof(DataMsg) + totalImgSize;
        }

        void Serialize(uint8_t *buffer) override {
            int off = 0;
            memcpy(buffer + off, &num, sizeof(num));
            off += (int) sizeof(num);
            memcpy(buffer + off, &fullPic, sizeof(MediaInfAiPic));
            int fullImgOff = off + (int) sizeof(MediaInfAiPic);
            if (fullPic.img && fullPic.size > 0) { memcpy(buffer + fullImgOff, fullPic.img, fullPic.size); }
            off += (fullPic.size + sizeof(MediaInfAiPic));

            int imgOff = off + num * (int) sizeof(DataMsg);
            for (int i = 0; i < num; i++) {
                if (array[i].pic.img && array[i].pic.size > 0) {
                    memcpy(buffer + imgOff, array[i].pic.img, array[i].pic.size);
                }
                array[i].imgOff = imgOff;
                memcpy(buffer + off, array + i, (int) sizeof(DataMsg));
                imgOff += array[i].pic.size;
                off += (int) sizeof(DataMsg);
            }
        }

        void Parse(const uint8_t *buffer, int size) override {
            int off = 0;
            memcpy(&num, buffer + off, sizeof(num));
            off += sizeof(num);
            Alloc(num);
            memcpy(&fullPic, buffer + off, (int) sizeof(MediaInfAiPic));
            off += (int) sizeof(MediaInfAiPic);
            if (fullPic.size > 0) {
                fullPic.img = (uint8_t *) EP_MALLOC(fullPic.size);
                if (fullPic.img) { memcpy(fullPic.img, buffer + off, fullPic.size); }
            }
            off += fullPic.size;
            if (num * (int) sizeof(DataMsg) + off <= size) {
                memcpy(array, buffer + off, num * (int) sizeof(DataMsg));
                for (int i = 0; i < num; i++) {
                    if (array[i].pic.size) {
                        array[i].pic.img = (uint8_t *) EP_MALLOC(array[i].pic.size);
                        if (array[i].pic.img) memcpy(array[i].pic.img, buffer + array[i].imgOff, array[i].pic.size);
                    }
                }
            }
        }

    public:
        int num;
        DataMsg *array;
        MediaInfAiPic fullPic;//背景图，根据配置决定是否有截图
    };
    /** @}*/
}// namespace Emx

#endif//EMX_MediaInfAiDataFaceRecognition_HPP