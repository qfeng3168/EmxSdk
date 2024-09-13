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
            Normal = 0, //!<正常模式
            LowPower, //!<低功耗模式
        };
        //!@brief aov的持久化参数
        struct Param {
            AovModeE mode;  //!<aov模式
            int intervalS;  //!<休眠持续时间，单位秒，
                            //!<-1:关闭定时器长期休眠
                            //!<0或者<-1：无效值
                            //!<其他：休眠时间
        };
    };
}// namespace Emx

#endif//EMX_MediaInfAov_HPP