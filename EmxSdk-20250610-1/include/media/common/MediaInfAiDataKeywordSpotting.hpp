//
// Created by tangfahong on 2025/01/16.
//

#ifndef EMX_MediaInfAiDataKeywordSpotting_HPP
#define EMX_MediaInfAiDataKeywordSpotting_HPP

#include "MediaInfAiData.hpp"
#include <cstring>

namespace Emx {
    /** @addtogroup media-common
     *  @{
     */

    class MediaInfAiDataKeywordSpotting : public MediaInfAiData {
    public:
        MediaInfAiDataKeywordSpotting() : MediaInfAiData() { memset(&msg, 0, sizeof(DataMsg)); }

    public:
        /*唤醒词类型*/
        enum TypeE {
            Awaken = 100,       //!<唤醒
            LightsOn = 101,     //!<开灯
            LightsOff = 102,    //!<关灯
            ScreenOn = 103,     //!<打开屏幕
            ScreenOff = 104,    //!<关闭屏幕
            CallPhone = 105,    //!<呼叫手机
            CallDevice = 106,   //!<呼叫设备
            HangUp = 107,       //!<挂断
            RestartDevice = 108,//!<重启设备
            PreviousSong = 109, //!<上一首
            NextSong = 110,     //!<下一首
            TellTime = 111,     //!<播报时间
            ReportWeather = 112,//!<播报天气
            Butt
        };
        struct DataMsg {
            TypeE type;//!<唤醒词类型
        };

        int GetSerializeSize() override { return (int) sizeof(DataMsg); }

        void Serialize(uint8_t *buffer) override { memcpy(buffer, &msg, (int) sizeof(DataMsg)); }

        void Parse(const uint8_t *buffer, int size) override { memcpy(&msg, buffer, (int) sizeof(DataMsg)); }

    public:
        DataMsg msg;
    };
    /** @}*/
}// namespace Emx

#endif//EMX_MediaInfAiDataKeywordSpotting_HPP