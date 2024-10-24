//
// Created by tangfahong on 2024/4/10.
//

#ifndef EMX_MediaInfAiDataVehicleRecognition_HPP
#define EMX_MediaInfAiDataVehicleRecognition_HPP

#include "MediaInfAiData.hpp"
#include "MediaRegion.hpp"
#include <cstring>

namespace Emx {

    class MediaInfAiDataVehicleRecognition : public MediaInfAiData {
    public:
        MediaInfAiDataVehicleRecognition() : MediaInfAiData(), num(0), array(nullptr) {
            memset(&fullPic, 0, sizeof(MediaInfAiPic));
        }

        virtual ~MediaInfAiDataVehicleRecognition() { Free(); }

    public:
        struct DataMsg {
            MediaInfAiPic pic;//!<车辆抠图，根据配置参数决定是否有截图
            MediaInfAiPic platePic;//!<车牌抠图，根据配置参数决定是否有截图
            uint64_t id;      //!<目标车辆id
            uint64_t plateId;      //!<目标车牌id
            Rect rect; //!<当前车辆位置信息
            Rect plateRect; //!<当前车牌位置信息
            /*目标类型*/
            enum class TypeE : uint32_t {
                LightCar,  //!<小型车
                HeavyCar,  //!<大型车
                Sedan,     //!<轿车
                SUV,       //!<越野车
                Van,       //!<货车
                Truck,     //!<卡车
                Motorcycle,//!<摩托车
                SportsCar, //!<跑车
                Pickup     //!<皮卡
            } type;
            uint32_t color;//!<目标rgb颜色
            uint32_t isRun;//!<运行中
            char brand[16];//!<品牌
            /*车牌颜色*/
            enum class PlateColorE : uint32_t {
                Blue,  //!<蓝色
                Yellow,//!<黄色
                White, //!<白色
                Black, //!<黑色
                Green  //!<绿色
            } plateColor;
            int plateLine;             //!<车牌行数，一般是1，个别车是2行
            uint8_t plateNumber[2][32];//!<车牌号码，一行占用一个下标，统一UTF-8编码
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
                    if ((array[i].platePic.size > 0) && array[i].platePic.img) {
                        free(array[i].platePic.img);
                        array[i].platePic.img = nullptr;
                        array[i].platePic.size = 0;
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
            for (int i = 0; i < num; i++) { 
                totalImgSize += array[i].pic.size;
                totalImgSize += array[i].platePic.size;
            }
            return (int) sizeof(num) + num * (int) sizeof(DataMsg) + totalImgSize;
        }

        void Serialize(uint8_t *buffer) override {
            int off = 0;
            memcpy(buffer + off, &num, sizeof(num));
            off += (int) sizeof(num);
            memcpy(buffer + off, &fullPic, sizeof(MediaInfAiPic));
            int fullImgOff = off + (int) sizeof(MediaInfAiPic);
            if (fullPic.img && fullPic.size > 0) {
                memcpy(buffer + fullImgOff, fullPic.img, fullPic.size);
            }
            off += (fullPic.size + sizeof(MediaInfAiPic));

            int imgOff = off + num * (int) sizeof(DataMsg);
            for (int i = 0; i < num; i++) {
                array[i].imgOff = imgOff;
                if (array[i].pic.img && array[i].pic.size > 0) {
                    memcpy(buffer + imgOff, array[i].pic.img, array[i].pic.size);
                }
                imgOff += array[i].pic.size;
                if (array[i].platePic.img && array[i].platePic.size > 0) {
                    memcpy(buffer + imgOff, array[i].platePic.img, array[i].platePic.size);
                }
                imgOff += array[i].platePic.size;
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
                    if (array[i].platePic.size) {
                        array[i].platePic.img = (uint8_t *) EP_MALLOC(array[i].platePic.size);
                        if (array[i].platePic.img) memcpy(array[i].platePic.img, buffer + imgOff, array[i].platePic.size);
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

#endif//EMX_MediaInfAiDataVehicleRecognition_HPP