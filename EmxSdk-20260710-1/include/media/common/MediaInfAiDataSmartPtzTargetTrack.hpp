//
// Created by tangfahong on 2026/07/10.
//

#ifndef EMX_MediaInfAiDataSmartPtzTargetTrack_HPP
#define EMX_MediaInfAiDataSmartPtzTargetTrack_HPP

#include "MediaInfAiData.hpp"
#include <cstring>

namespace Emx {
    /** @addtogroup media-common
     *  @{
     */

    class MediaInfAiDataSmartPtzTargetTrack : public MediaInfAiData {
    public:
        MediaInfAiDataSmartPtzTargetTrack() : MediaInfAiData() { memset(&msg, 0, sizeof(DataMsg)); }

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
        //!@brief 结束追踪类型
        enum class EndTypeE : uint8_t {
            Single,     //!<单次追踪结束
            TimeOut,    //!<超时结束
            Miss,       //!<缺失目标
            Static,     //!<目标静止结束
            FaceTrigger,//!<人脸触发结束
            UnDefined,  //!<未定义
        };
        struct TrackMsg {
            TypeE type;            //!<开始追踪类型
            int16_t angleH;        //!<追踪水平角度
            int16_t angleV;        //!<追踪垂直角度
            int32_t startTrackTime;//!<开始追踪时间
            int32_t endTrackTime;  //!<结束追踪时间
        };
        struct DataMsg {
            TrackMsg start;         //!<开始追踪
            TrackMsg stop;          //!<结束追踪
        };

        int GetSerializeSize() override { return (int) sizeof(DataMsg); }

        void Serialize(uint8_t *buffer) override { memcpy(buffer, &msg, (int) sizeof(DataMsg)); }

        void Parse(const uint8_t *buffer, int size) override { memcpy(&msg, buffer, (int) sizeof(DataMsg)); }

    public:
        DataMsg msg;
    };
    /** @}*/
}// namespace Emx

#endif//EMX_MediaInfAiDataSmartPtzTargetTrack_HPP