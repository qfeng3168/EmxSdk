//
// Created by xiong on 2023/8/16.
//

#ifndef EMX_MediaInfAenc_HPP
#define EMX_MediaInfAenc_HPP

#include "media/common/MediaTypedef.hpp"

namespace Emx {
    //!@brief 音频编码接口
    class MediaInfAenc {
    public:
        //!@brief 音频编码持久化参数
        struct Param {
            AudioCodecE codec; //!<编码类型
            int32_t bitRate; //!<码率
            int32_t sampleRate; //!<采样率
            int32_t volume; //!<音量大小
            uint8_t bitWidth; //!<采样位宽
            bool mute; //!<是否静音
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
    };
}

#endif //EMX_MediaInfAenc_HPP