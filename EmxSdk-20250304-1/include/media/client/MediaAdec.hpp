//
// Created by xiong on 2020/11/23.
//

#ifndef EMX_MEDIAADEC_HPP
#define EMX_MEDIAADEC_HPP

#include "media/common/MediaInfAdec.hpp"
#include "MediaParamCBase.hpp"
#include "core/EmxUtils.hpp"
namespace Emx {
    //!@brief 音频编码
    class MediaAdec : public MediaInfAdec, public MediaParamCBase<MediaInfAdec::Param> {
    public:
        explicit MediaAdec(int32_t chn) : MediaParamCBase(MediaModE::Adec, chn) {}

        ErrCodeE SetVolume(int32_t volume) override;

        ErrCodeE SetVolumeTemp(int32_t volume) override;

        PlayStatusE GetPlayStatus() override;
    };

    //!@brief 音频流解码接口,均为同步阻塞操作
    class MediaAdecStreamSync {
    public:
        /**
         * @brief 发送音频帧到媒体服务进行解码播放，这里发送的音频帧的编码格式需要与MediaClientAenc中配置的解码格式一致
         * @param chn [in] 目的通道号
         * @param frame [in] 与音频帧信息
         * @param timeoutMs [in] 超时时间，默认1s
         * @return ErrCode
         */
        static ErrCodeE PushFrame(int32_t chn, MediaAdec::Frame &frame, int32_t timeoutMs = 1000);

        /**
         * @brief 发送Pcm音频帧到媒体服务直接播放
         * @param chn [in] 目的通道号
         * @param frame [in] 音频帧信息
         * @param timeoutMs [in] 超时时间，默认1s
         * @return ErrCode
         */
        static ErrCodeE PushRawPcm(int32_t chn, MediaAdec::Frame &frame, int32_t timeoutMs = 1000);

        /**
         * @brief 添加音频文件路径至媒体服务的播放列表中，添加后此函数就会返回，列表为空时会立刻播放
         * @param chn [in] 目的通道号
         * @param path [in] 音频文件路径
         * @param timeoutMs [in] 超时时间，默认1s
         * @return ErrCode
         */
        static ErrCodeE PlayFile(int32_t chn, const char *path, int32_t timeoutMs = 1000);

        /**
         * @brief 添加音频文件路径至媒体服务的播放列表中，添加后此函数就会返回，列表为空时会立刻播放
         * @param chn [in] 目的通道号
         * @param path [in] 音频文件路径
         * @param volume [in] 希望临时以此音量播放文件
         * @param timeoutMs [in] 超时时间，默认1s
         * @return ErrCode
         */
        static ErrCodeE PlayFileWithVolume(int32_t chn, const char *path, int32_t volume, int32_t timeoutMs = 1000);

        /**
         * @brief 终止播放并清空媒体服务播放列表
         * @param chn [in] 目的通道号
         * @param fullInterrupt [in] 是否完全中断 默认true
         * @param timeoutMs [in] 超时时间，默认1s
         * @return ErrCode
         */
        static ErrCodeE PlayCtrlQuit(int32_t chn, bool fullInterrupt = true, int32_t timeoutMs = 1000);
    };

    //!@brief 音频流解码接口,均为同步阻塞操作,分离了创建-发送-销毁，相比于MediaAdecStreamSync减少频繁调用下的性能开销
    class MediaAdecStreamSync2 {
    public:
        /**
         * @brief 创建MsgTopicPublisherSync2
         * @return ErrCode
         */
        ErrCodeE Create() { return m_pub.Create(); }
        /**
         * @brief 销毁MsgTopicPublisherSync2
         */
        void Destroy() { m_pub.Destroy(); }
        /**
         * @brief 发送音频帧到媒体服务进行解码播放，这里发送的音频帧的编码格式需要与MediaClientAenc中配置的解码格式一致
         * @param chn [in] 目的通道号
         * @param frame [in] 与音频帧信息
         * @param timeoutMs [in] 超时时间，默认1s
         * @return ErrCode
         */
        ErrCodeE PushFrame(int32_t chn, MediaAdec::Frame &frame, int32_t timeoutMs = 1000);

        /**
         * @brief 发送Pcm音频帧到媒体服务直接播放
         * @param chn [in] 目的通道号
         * @param frame [in] 音频帧信息
         * @param timeoutMs [in] 超时时间，默认1s
         * @return ErrCode
         */
        ErrCodeE PushRawPcm(int32_t chn, MediaAdec::Frame &frame, int32_t timeoutMs = 1000);

        /**
         * @brief 添加音频文件路径至媒体服务的播放列表中，添加后此函数就会返回，列表为空时会立刻播放
         * @param chn [in] 目的通道号
         * @param path [in] 音频文件路径
         * @param timeoutMs [in] 超时时间，默认1s
         * @return ErrCode
         */
        ErrCodeE PlayFile(int32_t chn, const char *path, int32_t timeoutMs = 1000);

        /**
         * @brief 添加音频文件路径至媒体服务的播放列表中，添加后此函数就会返回，列表为空时会立刻播放
         * @param chn [in] 目的通道号
         * @param path [in] 音频文件路径
         * @param volume [in] 希望临时以此音量播放文件
         * @param timeoutMs [in] 超时时间，默认1s
         * @return ErrCode
         */
        ErrCodeE PlayFileWithVolume(int32_t chn, const char *path, int32_t volume, int32_t timeoutMs = 1000);

        /**
         * @brief 终止播放并清空媒体服务播放列表
         * @param chn [in] 目的通道号
         * @param fullInterrupt [in] 是否完全中断 默认true
         * @param timeoutMs [in] 超时时间，默认1s
         * @return ErrCode
         */
        ErrCodeE PlayCtrlQuit(int32_t chn, bool fullInterrupt = true, int32_t timeoutMs = 1000);

    private:
        MsgTopicPublisherSync2 m_pub;//!< 同步消息主题发布器
    };

    //!@brief 音频流解码异步接口,均为异步操作
    class MediaAdecStreamAsync {
    public:
        /**
         * @brief 创建异步音频解码流。
         * 
         * 该方法绑定到指定的事件循环，创建异步视频解码流。
         * @param loop 要绑定的事件循环对象。
         * @param buffSize 音频缓存区buffer大小，单位为字节。音频单包的最大值
         * @return 操作结果，成功返回 ErrCodeE::Success，失败返回相应错误码。
         */
        ErrCodeE Create(EuvLoop &loop, uint32_t buffSize);

        /**
         * @brief 发送音频帧到媒体服务进行解码播放，这里发送的音频帧的编码格式需要与MediaClientAenc中配置的解码格式一致
         * @param chn [in] 目的通道号
         * @param frame [in] 与音频帧信息
         * @return ErrCode
         */
        ErrCodeE PushFrame(int32_t chn, MediaAdec::Frame &frame);
        /**
         * @brief 发送Pcm音频帧到媒体服务直接播放
         * @param chn [in] 目的通道号
         * @param frame [in] 音频帧信息
         * @return ErrCode
         */
        ErrCodeE PushRawPcm(int32_t chn, MediaAdec::Frame &frame);

        /**
         * @brief 销毁异步音频解码流。
         */
        void Destroy();

    private:
        /**
         * @brief 发送音频帧到媒体服务进行解码播放，这里发送的音频帧的编码格式需要与MediaClientAenc中配置的解码格式一致
         * @param topic [in] 目的订阅号
         * @param frame [in] 与音频帧信息
         * @return ErrCode
         */
        ErrCodeE PushData(uint32_t topic, MediaAdec::Frame &frame);

    private:
        MsgTopicPublisherAsync m_pub;//!< 异步消息主题发布器
        int m_chn;                   //!< 解码通道号
        uint8_t *m_buffer;           //!< 缓冲区
        uint32_t m_bufferSize;       //!< 缓冲区大小
        uint32_t m_userSize;         //!< 用户数据缓冲区大小
    };
}// namespace Emx

#endif//EMX_MEDIAADEC_HPP
