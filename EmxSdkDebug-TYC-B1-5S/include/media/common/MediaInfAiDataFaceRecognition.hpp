//
// Created by tangfahong on 2024/4/16.
//

#ifndef EMX_MediaInfAiDataFaceRecognition_HPP
#define EMX_MediaInfAiDataFaceRecognition_HPP

#include "MediaInfAiData.hpp"
#include "MediaRegion.hpp"
#include <cstring>

namespace Emx {

    class MediaInfAiDataFaceRecognition : public MediaInfAiData {
    public:
        MediaInfAiDataFaceRecognition() : MediaInfAiData(), num(0), array(nullptr) {
            memset(&fullPic, 0, sizeof(MediaInfAiPic));
        }

        virtual ~MediaInfAiDataFaceRecognition() { Free(); }

    public:
        struct DataMsg {
            MediaInfAiPic pic;
            int32_t id;        //!<人脸库id
            uint8_t quality;   //!<人脸质量[0~100]
            uint8_t sharpness; //!<人脸清晰度[0~100]
            uint8_t similarity;//!<人脸相似度[0~100]
            uint8_t expression;//!<人脸表情,预留
            int32_t ipd;       //!<人脸瞳距
            Rect rect;         //!<当前人脸位置信息
            int imgOff;        //!<私有数据，记录内存偏移
        };

        void Alloc(int n) {
            Free();
            array = (DataMsg *) malloc(n * (int) sizeof(DataMsg));
            if (array) { memset((void *) array, 0, n * sizeof(DataMsg)); }
        }

        void Free() {
            if (array) {
                free(array);
                array = nullptr;
            }
        }

        void ReleaseImg() {
            for (int i = 0; i < num; i++) {
                if (array[i].pic.img) { free(array[i].pic.img); }
            }
            if (fullPic.img) { free(fullPic.img); }
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

#endif//EMX_MediaInfAiDataFaceRecognition_HPP