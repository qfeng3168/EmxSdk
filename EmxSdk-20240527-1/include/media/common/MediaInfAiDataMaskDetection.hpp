//
// Created by xiong on 2023/9/13.
//

#ifndef EMX_MediaInfAiDataMaskDetection_HPP
#define EMX_MediaInfAiDataMaskDetection_HPP

#include "MediaInfAiData.hpp"
#include "MediaRegion.hpp"
#include <cstring>

namespace Emx {

    class MediaInfAiDataMaskDetection : public MediaInfAiData {
    public:
        MediaInfAiDataMaskDetection() : MediaInfAiData(), num(0), array(nullptr) {
            memset(&fullPic, 0, sizeof(MediaInfAiPic));
        }

        virtual ~MediaInfAiDataMaskDetection() { Free(); }

    public:
        struct DataMsg {
            MediaInfAiPic pic;
            uint64_t id;         //!<人脸id
            uint64_t captureTime;//!<人脸抓拍到的毫秒时间戳
            uint64_t startTime;  //!<人脸开始出现时间的毫秒时间戳
            uint64_t endTime;    //!<人脸结束时间,毫秒时间戳
            int32_t quality;     //!<人脸质量
            int32_t sharpness;   //!<人脸清晰度
            int32_t isMask;      //!<是否佩戴口罩
            uint32_t maskColor;  //!<口罩颜色
            uint32_t maskType;   //!<口罩类型
            Rect rect;           //!<当前人脸位置信息
            int imgOff;          //!<私有数据，记录内存偏移
        };

        void Alloc(int n) {
            Free();
            array = (DataMsg *) malloc(n * (int) sizeof(DataMsg));
            if (array) { memset((void *) array, 0, n * sizeof(DataMsg)); }
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
            memcpy(buffer + fullImgOff, fullPic.img, fullPic.size);
            off += (fullPic.size + sizeof(MediaInfAiPic));

            int imgOff = off + num * (int) sizeof(DataMsg);
            for (int i = 0; i < num; i++) {
                memcpy(buffer + imgOff, array[i].pic.img, array[i].pic.size);
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
            memcpy(&fullPic, buffer + off, (int) sizeof(MediaInfAiPic));
            off += (int) sizeof(MediaInfAiPic);
            if (fullPic.size > 0) {
                fullPic.img = (uint8_t *) malloc(fullPic.size);
                if (fullPic.img) { memcpy(fullPic.img, buffer + off, fullPic.size); }
            }
            off += fullPic.size;
            if (num * (int) sizeof(DataMsg) + off <= size) {
                Alloc(num);
                memcpy(array, buffer + off, num * (int) sizeof(DataMsg));
                for (int i = 0; i < num; i++) {
                    if (array[i].pic.size) {
                        array[i].pic.img = (uint8_t *) malloc(array[i].pic.size);
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
}// namespace Emx

#endif//EMX_MediaInfAiDataMaskDetection_HPP