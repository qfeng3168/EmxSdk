//
// Created by xiong on 2023/8/22.
//

#ifndef EMX_MediaInfAdec_HPP
#define EMX_MediaInfAdec_HPP

#include "media/common/MediaTypedef.hpp"

namespace Emx {

    //!@brief 音频解码
    class MediaInfAdec {
    public:
        //!@brief 音频解码持久化参数
        struct Param {
            AudioCodecE codec; //!<编码类型
            int32_t bitRate; //!<码率
            int32_t sampleRate; //!<采样率
            int32_t volume; //!<音量大小
            uint8_t bitWidth; //!<采样位宽
        };

        //!@brief 音频解码帧结构定义(目前未使用MediaFrame结构)
        struct Frame {
            uint64_t tsInUs; //!<微秒级时间戳
            uint32_t seq; //!<帧序号
            int32_t size; //!<数据大小
            uint8_t *data; //!<数据内存地址
        };

        //!@brief 播放状态
        enum PlayStatusE : uint32_t {
            Unknown = 0, //!<未知
            Playing,     //!<播放中
            Finish,      //!<播放结束
        };

        /**
         * @brief 设置音量
         * @param volume [in] 音量大小
         * @return ErrCodeE
         */
        virtual ErrCodeE SetVolume(int32_t volume) = 0;

        /**
         * @brief 设置临时音量，调用这个接口配置的音量并不会被存储记录，重启后会失效
         * @param volume [in] 音量大小
         * @return ErrCodeE
         */
        virtual ErrCodeE SetVolumeTemp(int32_t volume) = 0;

        /**
         * @brief 获取媒体音频播放状态
         * @return PlayStatusE
         */
        virtual PlayStatusE GetPlayStatus() = 0;
    };
}

#endif //EMX_MediaInfAdec_HPP