//
// Created by tangfahong on 2024/07/20.
//

#ifndef EMX_MediaInfAiDataClassify_HPP
#define EMX_MediaInfAiDataClassify_HPP

#include "MediaInfAiData.hpp"
#include "MediaRegion.hpp"
#include <cstring>

namespace Emx {
    /** @addtogroup media-common
     *  @{
     */

    class MediaInfAiDataClassify : public MediaInfAiData {
    public:
        MediaInfAiDataClassify() : MediaInfAiData(), num(0), array(nullptr) {
            memset(&fullPic, 0, sizeof(MediaInfAiPic));
        }

        virtual ~MediaInfAiDataClassify() { Free(); }

    public:
        enum TypeE {
            None = 0,           //!< 默认无效
            Face = 1,           //!< 人脸
            Person = 2,         //!< 人
            HumanHead = 3,      //!< 人头
            Bicycle = 4,        //!< 自行车
            Car = 5,            //!< 汽车
            Motorcycle = 6,     //!< 摩托车
            Bus = 7,            //!< 公交车
            Truck = 8,          //!< 卡车
            Train = 9,          //!< 火车
            Bird = 10,          //!< 鸟
            Cat = 11,           //!< 猫
            Dog = 12,           //!< 狗
            Horse = 13,         //!< 马
            Cow = 14,           //!< 牛
            Sheep = 15,         //!< 羊
            Pig = 16,           //!< 猪
            Rabbit = 17,        //!< 兔
            Duck = 18,          //!< 鸭
            Goose = 19,         //!< 鹅
            Chicken = 20,       //!< 鸡
            Fish = 21,          //!< 鱼
            Elephant = 22,      //!< 大象
            Lion = 23,          //!< 狮子
            Tiger = 24,         //!< 老虎
            Monkey = 25,        //!< 猴子
            Panda = 26,         //!< 熊猫
            Giraffe = 27,       //!< 长颈鹿
            Snake = 28,         //!< 蛇
            Tree = 29,          //!< 树
            Flower = 30,        //!< 花
            Grass = 31,         //!< 草
            Mushroom = 32,      //!< 蘑菇
            Cactus = 33,        //!< 仙人掌
            Boat = 34,          //!< 船
            Submarine = 35,     //!< 潜艇
            Airplane = 36,      //!< 飞机
            Helicopter = 37,    //!< 直升机
            Bed = 38,           //!< 床
            Sofa = 39,          //!< 沙发
            Table = 40,         //!< 桌子
            Chair = 41,         //!< 椅子
            Wardrobe = 42,      //!< 衣柜
            Television = 43,    //!< 电视
            Refrigerator = 44,  //!< 冰箱
            WashingMachine = 45,//!< 洗衣机
            MicrowaveOven = 46, //!< 微波炉
            Computer = 47,      //!< 电脑
            MobilePhone = 48,   //!< 手机
            House = 49,         //!< 房子
            Villa = 50,         //!< 别墅
            School = 51,        //!< 学校
            Hospital = 52,      //!< 医院
            Library = 53,       //!< 图书馆
            ShoppingMall = 54,  //!< 商场
            Stadium = 55,       //!< 体育馆
            Eye = 56,           //!< 眼睛
            Nose = 57,          //!< 鼻子
            Mouth = 58,         //!< 嘴巴
            Hand = 59,          //!< 手
            Foot = 60,          //!< 脚
            Apple = 61,         //!< 苹果
            Banana = 62,        //!< 香蕉
            Orange = 63,        //!< 橙子
            Grape = 64,         //!< 葡萄
            Water = 65,         //!< 水
            Milk = 66,          //!< 牛奶
            Coffee = 67,        //!< 咖啡
            Tea = 68,           //!< 茶
            Lake = 69,          //!< 湖泊
            Ocean = 70,         //!< 海洋
            Pond = 71,          //!< 池塘
            River = 72,         //!< 河流
            Stream = 73,        //!< 溪流
            Flame = 74,         //!< 火焰
            Smoke = 75,         //!< 烟雾
            End
        };

        struct DataMsg {
            TypeE type;
            MediaInfAiPic pic;//!<目标抠图，根据配置参数决定是否有截图
            uint64_t id;      //!<目标id
            Rect rect;        //!<当前目标位置信息
            int imgOff;       //!<私有数据，记录内存偏移
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
    /** @}*/
}// namespace Emx

#endif//EMX_MediaInfAiDataClassify_HPP