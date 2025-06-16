//
// Created by tangfahong on 2024/9/12.
//

#ifndef EMX_MediaInfAov_HPP
#define EMX_MediaInfAov_HPP

#include "media/common/MediaTypedef.hpp"

namespace Emx {
    //!@brief Aov接口
    class MediaInfAov {
    public:
        //!@brief aov模式枚举
        enum class AovModeE : uint8_t {
            Normal = 0,//!<正常模式
            LowPower,  //!<低功耗模式
        };
        //!@brief aov的持久化参数
        struct Param {
            AovModeE mode;//!<aov模式
            int intervalS;//!<休眠持续时间，单位秒，
                          //!<-1:关闭定时器长期休眠
                          //!<0或者<-1：无效值
                          //!<其他：休眠时间
            int TargetLostTimeOutFps;//!<目标丢失超时帧率，单位帧率，
                                     //!<0或者<-1：无效值
                                     //!<其他：目标丢失超时帧率
        };
        //!@brief aov状态类型
        enum class AovStatusTypeE : uint8_t {
            VideoFrameSuccessful = 0,//!<完整视频帧获取成功
            VideoFrameFailed,        //!<完整视频帧获取失败
            TargetTrigger,           //!<目标触发（人形侦测触发）
            TargetLost,              //!<目标丢失（人形侦测丢失）
        };

        //!@brief aov推送类型
        enum class AovPushTypeE : uint8_t {
            SleepReady = 0,//!<休眠准备
            HighFrameRate, //!<高帧率状态
            LowFrameRate,  //!<低帧率状态
            UartData,      //!<uart数据
        };

        struct AovPushFrame {
            AovPushTypeE type; //!<aov推送类型
            int32_t size; //!<数据大小
            uint8_t data[128]; //!<数据
        };
        /**
         * @brief 更新aov状态
         * @param type [in] 状态类型
         * @param flag [in] 状态值
         * @return ErrCodeE
         */
        virtual ErrCodeE UpdateStatus(AovStatusTypeE type) = 0;
    };
}// namespace Emx

#endif//EMX_MediaInfAov_HPP