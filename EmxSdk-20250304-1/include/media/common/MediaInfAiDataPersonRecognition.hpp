//
// Created by tangfahong on 2024/4/10.
//

#ifndef EMX_MediaInfAiDataPersonRecognition_HPP
#define EMX_MediaInfAiDataPersonRecognition_HPP

#include "MediaInfAiData.hpp"
#include "MediaRegion.hpp"
#include <cstring>

namespace Emx {

    class MediaInfAiDataPersonRecognition : public MediaInfAiData {
    public:
        MediaInfAiDataPersonRecognition() : MediaInfAiData(), num(0), array(nullptr) {
            memset(&fullPic, 0, sizeof(MediaInfAiPic));
        }

        virtual ~MediaInfAiDataPersonRecognition() { Free(); }

    public:
        struct DataMsg {
            MediaInfAiPic pic;//!<人形抠图，根据配置参数决定是否有截图
            uint64_t id;      //!<目标id
            Rect rect;        //!<人形位置信息
            /*衣服类型*/
            enum class ClothesTypeE : uint32_t {
                Shirts,     //!<衬衫
                TShirts,    //!<T 恤
                Hoodies,    //!<卫衣
                Coats,      //!<外套
                Windbreakers//!<风衣
            } clothesType;
            uint32_t clothesColor;//!<衣服rgb颜色
            /*下装类型*/
            enum class BottomsTypeE : uint32_t {
                Skirts,//!<裙子
                Shorts,//!<短裤
                Pants  //!<长裤
            } bottomsType;
            uint32_t bottomsColor;//!<下装rgb颜色
            /*人的行为类型*/
            enum class BehaviorTypeE : uint32_t {
                Standing, //!<站立
                squatting,//!<蹲着
                lying,    //!<躺着
                running,  //!<跑
                walking   //!<慢走
            } behaviorType;
            KeyPoint keyPoints[32];//!<人体关键点
            int keyPointNums;      //!<人体关键点数量
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
                array[i].imgOff = imgOff;
                if (array[i].pic.img && array[i].pic.size > 0) {
                    memcpy(buffer + imgOff, array[i].pic.img, array[i].pic.size);
                }
                imgOff += array[i].pic.size;
                memcpy(buffer + off, array + i, (int) sizeof(DataMsg));
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
                    int imgOff = array[i].imgOff;
                    if (array[i].pic.size) {
                        array[i].pic.img = (uint8_t *) EP_MALLOC(array[i].pic.size);
                        if (array[i].pic.img) memcpy(array[i].pic.img, buffer + imgOff, array[i].pic.size);
                        imgOff += array[i].pic.size;
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

#endif//EMX_MediaInfAiDataPersonRecognition_HPP