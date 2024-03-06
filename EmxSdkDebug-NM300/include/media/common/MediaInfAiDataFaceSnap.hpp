//
// Created by xiong on 2023/9/13.
//

#ifndef EMX_MediaInfAiDataFaceSnap_HPP
#define EMX_MediaInfAiDataFaceSnap_HPP

#include "MediaInfAiData.hpp"
#include "MediaRegion.hpp"
#include <cstring>

namespace Emx {

    class MediaInfAiDataFaceSnap : public MediaInfAiData {
    public:
        MediaInfAiDataFaceSnap() : MediaInfAiData(), num(0), array(nullptr) {}

        virtual ~MediaInfAiDataFaceSnap() {
            Free();
        }

    public:
        struct FaceMsg {
            MediaInfAiPic pic;
            bool isFullPic;
            uint64_t id; /*人脸id*/
            uint64_t captureTime; /*人脸抓拍到的毫秒时间戳*/
            uint64_t startTime; /*人脸开始出现时间的毫秒时间戳*/
            uint64_t endTime; /*人脸结束时间,毫秒时间戳*/
            int32_t quality; /*人脸质量*/
            int32_t sharpness; /*人脸清晰度*/
            Rect rect; /*当前人脸位置信息*/
            int imgOff; /*私有数据，记录内存偏移*/
        };

        void Alloc(int n) {
            Free();
            array = (FaceMsg *) malloc(n * (int) sizeof(FaceMsg));
        }

        void Free() {
            if (array) {
                free(array);
                array = nullptr;
            }
        }

        void ReleaseImg() {
            for (int i = 0; i < num; i++) {
                free(array[i].pic.img);
            }
        }

        int GetSerializeSize() override {
            int totalImgSize = 0;
            for (int i = 0; i < num; i++) {
                totalImgSize += array[i].pic.size;
            }
            return (int) sizeof(num) + num * (int) sizeof(FaceMsg) + totalImgSize;
        }

        void Serialize(uint8_t *buffer) override {
            int off = 0;
            memcpy(buffer + off, &num, sizeof(num));
            off += (int) sizeof(num);
            int imgOff = off + num * (int) sizeof(FaceMsg);
            for (int i = 0; i < num; i++) {
                memcpy(buffer + imgOff, array[i].pic.img, array[i].pic.size);
                array[i].imgOff = imgOff;
                memcpy(buffer + off, array + i, (int) sizeof(FaceMsg));
                imgOff += array[i].pic.size;
                off += (int) sizeof(FaceMsg);
            }
        }

        void Parse(const uint8_t *buffer, int size) override {
            int off = 0;
            memcpy(&num, buffer + off, sizeof(num));
            off += sizeof(num);
            if (num * (int) sizeof(FaceMsg) + off < size) {
                Alloc(num);
                memcpy(array, buffer + off, num * (int) sizeof(FaceMsg));
                for (int i = 0; i < num; i++) {
                    array[i].pic.img = (uint8_t *) malloc(array[i].pic.size);
                    memcpy(array[i].pic.img, buffer + array[i].imgOff, array[i].pic.size);
                }
            }
        }

    public:
        int num;
        FaceMsg *array;
    };
}

#endif //EMX_MediaInfAiDataFaceSnap_HPP