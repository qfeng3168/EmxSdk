//
// Created by tangfahong on 2024/07/20.
//

#ifndef EMX_MediaInfAiDataRegionalPeopleStatistics_HPP
#define EMX_MediaInfAiDataRegionalPeopleStatistics_HPP

#include "MediaInfAiData.hpp"
#include "MediaRegion.hpp"
#include <cstring>

namespace Emx {
    /** @addtogroup media-common
     *  @{
     */

    class MediaInfAiDataRegionalPeopleStatistics : public MediaInfAiData {
    public:
        MediaInfAiDataRegionalPeopleStatistics() : MediaInfAiData() { memset(&msg, 0, sizeof(DataMsg)); }

    public:
        struct DataMsg {
            uint32_t peopleNum;//!<当前人数
        };

        int GetSerializeSize() override { return (int) sizeof(DataMsg); }

        void Serialize(uint8_t *buffer) override { memcpy(buffer, &msg, (int) sizeof(DataMsg)); }

        void Parse(const uint8_t *buffer, int size) override { memcpy(&msg, buffer, (int) sizeof(DataMsg)); }

    public:
        DataMsg msg;
    };
    /** @}*/
}// namespace Emx

#endif//EMX_MediaInfAiDataRegionalPeopleStatistics_HPP