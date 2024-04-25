//
// Created by tangfahong on 2024/3/1.
//

#ifndef EMX_MediaInfAiDataPassengerFlowStatistics_HPP
#define EMX_MediaInfAiDataPassengerFlowStatistics_HPP

#include "MediaInfAiData.hpp"
#include "MediaRegion.hpp"
#include <cstring>

namespace Emx {

    class MediaInfAiDataPassengerFlowStatistics : public MediaInfAiData {
    public:
        MediaInfAiDataPassengerFlowStatistics() : MediaInfAiData() { memset(&msg, 0, sizeof(DataMsg)); }

    public:
        struct DataMsg {
            uint32_t entryNum;//!<进入人数
            uint32_t leaveNum;//!<离开人数
        };

        int GetSerializeSize() override { return (int) sizeof(DataMsg); }

        void Serialize(uint8_t *buffer) override { memcpy(buffer, &msg, (int) sizeof(DataMsg)); }

        void Parse(const uint8_t *buffer, int size) override { memcpy(&msg, buffer, (int) sizeof(DataMsg)); }

    public:
        DataMsg msg;
    };
}// namespace Emx

#endif//EMX_MediaInfAiDataPassengerFlowStatistics_HPP