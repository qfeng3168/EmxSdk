//
// Created by tangfahong on 2024/08/08.
//

#ifndef EMX_MediaInfAiDataBabyCryDetection_HPP
#define EMX_MediaInfAiDataBabyCryDetection_HPP

#include "MediaInfAiData.hpp"
#include <cstring>

namespace Emx {

    class MediaInfAiDataBabyCryDetection : public MediaInfAiData {
    public:
        MediaInfAiDataBabyCryDetection() : MediaInfAiData() { memset(&msg, 0, sizeof(DataMsg)); }

    public:
        struct DataMsg {
            uint8_t type;    //!<啼哭类型-预留
            uint8_t soundDb; //!<音量db值-预留
            uint16_t babyNum;//!<婴儿数量-预留
        };

        int GetSerializeSize() override { return (int) sizeof(DataMsg); }

        void Serialize(uint8_t *buffer) override { memcpy(buffer, &msg, (int) sizeof(DataMsg)); }

        void Parse(const uint8_t *buffer, int size) override { memcpy(&msg, buffer, (int) sizeof(DataMsg)); }

    public:
        DataMsg msg;
    };
}// namespace Emx

#endif//EMX_MediaInfAiDataBabyCryDetection_HPP