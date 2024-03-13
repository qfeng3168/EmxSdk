//
// Created by xiong on 2023/8/22.
//

#ifndef EMX_MediaInfVenc_HPP
#define EMX_MediaInfVenc_HPP

#include "media/common/MediaTypedef.hpp"

namespace Emx {

    class MediaInfVenc {
    public:
        //!@brief 视频编码码率控制类型
        enum class RcModeE : uint8_t {
            CBR = 0, //!<固定码率
            VBR, //!<可变码率
            AVBR, //!<平均可变码率
            BUTT,
        };

        //!@brief 视频编码持久化参数
        struct Param {
            VideoCodecE codec; //!<编码类型
            uint16_t width; //!<图像宽
            uint16_t height; //!<图像高
            uint16_t fps; //!<帧率
            uint16_t iFrameInv; //!<I帧间隔单位s
            uint32_t bitRate; //!<码率kbps
            RcModeE rcMode; //!<码率控制模式
            bool drawRect; //!<是否画框
        };

        /**
         * @brief 刷新I帧
         * @return ErrCodeE
         */
        virtual ErrCodeE FlushIDR() = 0;
    };

}

#endif //EMX_MediaInfVenc_HPP