//
// Created by tangfahong on 2024/08/08.
//

#ifndef EMX_MediaInfAiDataPtzTargetTrack_HPP
#define EMX_MediaInfAiDataPtzTargetTrack_HPP

#include "MediaInfAiData.hpp"
#include <cstring>

namespace Emx {
    /** @addtogroup media-common
     *  @{
     */

    class MediaInfAiDataPtzTargetTrack : public MediaInfAiData {
    public:
        MediaInfAiDataPtzTargetTrack() : MediaInfAiData() { memset(&msg, 0, sizeof(DataMsg)); }

    public:
        //!@brief 追踪类型
        enum class TypeE : uint8_t {
            Motion,         //!<移动
            Person,         //!<人形
            Vehicles,       //!<机动车辆
            NonMotorVehicle,//!<非机动车辆
            Pets,           //!<宠物
            Face,           //!<人脸
            UnDefined,      //!<未定义
        };
        struct DataMsg {
            TypeE type;          //!<当前追踪类型
            uint32_t stopTimeOut;//!<停止追踪时间
        };

        int GetSerializeSize() override { return (int) sizeof(DataMsg); }

        void Serialize(uint8_t *buffer) override { memcpy(buffer, &msg, (int) sizeof(DataMsg)); }

        void Parse(const uint8_t *buffer, int size) override { memcpy(&msg, buffer, (int) sizeof(DataMsg)); }

    public:
        DataMsg msg;
    };
    /** @}*/
}// namespace Emx

#endif//EMX_MediaInfAiDataPtzTargetTrack_HPP