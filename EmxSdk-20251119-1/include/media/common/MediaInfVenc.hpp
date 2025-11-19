//
// Created by xiong on 2023/8/22.
//

#ifndef EMX_MediaInfVenc_HPP
#define EMX_MediaInfVenc_HPP

#include "media/common/MediaTypedef.hpp"

namespace Emx {
    /** @addtogroup media-common
     *  @{
     */

    class MediaInfVenc {
    public:
        //!@brief 视频编码码率控制类型
        enum class RcModeE : uint8_t {
            CBR = 0,//!<固定码率
            VBR,    //!<可变码率
            AVBR,   //!<平均可变码率
            FIXQP,  //!<固定qp
            BUTT,
        };

        //!@brief 视频编码持久化参数
        struct Param {
            VideoCodecE codec;     //!<编码类型
            uint16_t width;        //!<图像宽
            uint16_t height;       //!<图像高
            uint16_t fps;          //!<帧率
            uint16_t iFrameInv;    //!<I帧间隔单位s
            uint32_t bitRate;      //!<平均码率kbps
            uint32_t bitRateStatic;//!<静态码率
            uint32_t bitRatePeak;  //!<峰值码率
            RcModeE rcMode;        //!<码率控制模式
            bool drawRect;         //!<是否画框
            int32_t qualityLevel;  //!<编码质量等级 挡位0~10 0=自适应
            bool smartCode;        //!< 智能编码开关
            int8_t fixQp;          //!< 固定qp值
            int8_t profile;        //!< 视频编码配置文件：
            //!< H264：0=Baseline Profile（基础配置），1=Main Profile（主要配置），2=High Profile（高级配置）
            //!< H265：0=Main Profile（主要配置），1=Main 10 Profile(支持 10 位色深)，2=Main Still Picture Profile：（专为静态图片优化，支持单帧高分辨率编码）
            int8_t reserved[2];     //!< 保留字段，对齐
        };

        /**
         * @brief 刷新I帧
         * @return ErrCodeE
         */
        virtual ErrCodeE FlushIDR() = 0;
    };

    /** @}*/
}// namespace Emx

#endif//EMX_MediaInfVenc_HPP