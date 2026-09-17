//
// Created by tangfahong on 2024/07/20.
//

#ifndef EMX_MediaInfAiDataFlameSmokeDetection_HPP
#define EMX_MediaInfAiDataFlameSmokeDetection_HPP

#include "MediaInfAiData.hpp"
#include "MediaRegion.hpp"
#include <cstring>

namespace Emx {
    /** @addtogroup media-common
     *  @{
     */

    class MediaInfAiDataFlameSmokeDetection : public MediaInfAiData {
    public:
        MediaInfAiDataFlameSmokeDetection() : MediaInfAiData(), num(0), array(nullptr) {}

        virtual ~MediaInfAiDataFlameSmokeDetection() {Free(); }
    public:
        enum TypeE {
            Flame = 0,       //!<火焰
            Smoke = 1,     //!<烟雾
            Butt
        };
        struct DataMsg {
            TypeE type;
            Rect rect; //!<当前目标位置信息
        };
        void Alloc(int n) {
            Free();
            array = (DataMsg *) EP_MALLOC(n * (int) sizeof(DataMsg));
            if (array) { memset((void *) array, 0, n * sizeof(DataMsg)); }
        }

        void Free() {
            if (array) {
                free(array);
                array = nullptr;
            }
        }

        int GetSerializeSize() override { return (int) sizeof(num) + num * (int) sizeof(DataMsg); }

        void Serialize(uint8_t *buffer) override {
            int off = 0;
            memcpy(buffer + off, &num, sizeof(num));
            off += sizeof(num);
            memcpy(buffer + off, array, num * (int) sizeof(DataMsg));
        }

        void Parse(const uint8_t *buffer, int size) override {
            int off = 0;
            memcpy(&num, buffer + off, sizeof(num));
            off += sizeof(num);
            if (num && num * (int) sizeof(DataMsg) + off == size) {
                Alloc(num);
                memcpy(array, buffer + off, num * (int) sizeof(DataMsg));
            }
        }

    public:
        int num;
        DataMsg *array;
    };
    /** @}*/
}// namespace Emx

#endif//EMX_MediaInfAiDataFlameSmokeDetection_HPP