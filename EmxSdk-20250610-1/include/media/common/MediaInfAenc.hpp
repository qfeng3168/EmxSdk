//
// Created by xiong on 2023/8/16.
//

#ifndef EMX_MediaInfAenc_HPP
#define EMX_MediaInfAenc_HPP

#include "media/common/MediaTypedef.hpp"

namespace Emx {
    /** @addtogroup media-common
     *  @{
     */
    //!@brief 音频编码接口
    class MediaInfAenc {
    public:
        //!@brief 音频编码持久化参数
        struct Param {
            AudioCodecE codec; //!<编码类型
            int32_t bitRate;   //!<码率
            int32_t sampleRate;//!<采样率
            int32_t volume;    //!<音量大小
            uint8_t bitWidth;  //!<采样位宽
            bool aec;          //!回声消除
            bool mute;         //!<是否静音
            int chnNum;        //!<通道数量 1:单通道 2:双通道（立体声）
        };

        /**
         * @brief 设置音量
         * @param volume [in] 音量大小
         * @return ErrCodeE
         */
        virtual ErrCodeE SetVolume(int32_t volume) = 0;

        /**
         * @brief 设置静音
         * @param ena [in] 是否静音
         * @return ErrCodeE
         */
        virtual ErrCodeE Mute(bool ena) = 0;

        /*!
         * @brief 音频开关控制接口
         * @param onOff [in] 开关控制
         * @note 该接口需要实现，用于控制音频的开关
         * return ErrCodeE
         */
        virtual ErrCodeE OnOffCtrl(bool onOff) = 0;
    };
    /** @}*/
}// namespace Emx

#endif//EMX_MediaInfAenc_HPP