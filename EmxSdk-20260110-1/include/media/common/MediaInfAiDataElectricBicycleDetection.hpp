//
// Created by tangfahong on 2025/9/22.
//

#ifndef EMX_MediaInfAiDataElectricBicycleDetection_HPP
#define EMX_MediaInfAiDataElectricBicycleDetection_HPP

#include "MediaInfAiData.hpp"
#include "MediaRegion.hpp"
#include <cstring>
#include "EmxCore.hpp"

namespace Emx {
    /** @addtogroup media-common
     *  @{
     */

    class MediaInfAiDataElectricBicycleDetection : public MediaInfAiData {
    public:
        MediaInfAiDataElectricBicycleDetection() : MediaInfAiData(), num(0), array(nullptr) {
            memset(&fullPic, 0, sizeof(MediaInfAiPic));
        }

        virtual ~MediaInfAiDataElectricBicycleDetection() { Free(); }

    public:
        struct DataMsg {
            MediaInfAiPic pic;//!<目标抠图，根据配置参数决定是否有截图
            uint64_t id;      //!<目标id
            /*目标类型*/
            enum class TypeE : uint8_t {
                Bicycle = 0,//!<电动自行车
                Motorbike,  //!<电动摩托车
                Tricycle,   //!<电动三轮车
            } type;
            /*电动车状态*/
            enum class StateE : uint8_t {
                Parked = 0, //!<电动车停在车位
                Biked,      //!<电动车骑行中
                PushedAlong,//!<电动车被推着走
            } state;

            uint8_t quality;//!<当前电动车抓拍质量[0~100]
            uint8_t reserved;//!< 预留

            Rect rect; //!<当前目标位置信息
            int imgOff;//!<私有数据，记录内存偏移
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

#endif//EMX_MediaInfAiDataElectricBicycleDetection_HPP