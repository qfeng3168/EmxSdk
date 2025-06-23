//
// Created by tangfahong on 2024/07/20.
//

#ifndef EMX_MediaInfAiDataOffDutyDetection_HPP
#define EMX_MediaInfAiDataOffDutyDetection_HPP

#include "MediaInfAiData.hpp"
#include "MediaRegion.hpp"
#include <cstring>

namespace Emx {
    /** @addtogroup media-common
     *  @{
     */

    class MediaInfAiDataOffDutyDetection : public MediaInfAiData {
    public:
        MediaInfAiDataOffDutyDetection() : MediaInfAiData() { memset(&msg, 0, sizeof(DataMsg)); }

    public:
        struct DataMsg {
            uint32_t timeOut;//!<离岗超时时间，单位秒
            uint32_t peopleNum;  //!<当前在岗人数
        };

        int GetSerializeSize() override { return (int) sizeof(DataMsg); }

        void Serialize(uint8_t *buffer) override { memcpy(buffer, &msg, (int) sizeof(DataMsg)); }

        void Parse(const uint8_t *buffer, int size) override { memcpy(&msg, buffer, (int) sizeof(DataMsg)); }

    public:
        DataMsg msg;
    };
    /** @}*/
}// namespace Emx

#endif//EMX_MediaInfAiDataOffDutyDetection_HPP