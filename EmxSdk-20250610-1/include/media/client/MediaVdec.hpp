//
// Created by tfh on 2024/01/06.
//

#ifndef EMX_MediaVdec_HPP
#define EMX_MediaVdec_HPP

#include "media/common/MediaInfVdec.hpp"
#include "MediaParamCBase.hpp"
#include "media/common/MediaRingBuffer.hpp"

namespace Emx {
    /**
     * @brief 视频解码类，实现视频解码相关功能。
     * 
     * 该类继承自 MediaInfVdec 和 MediaParamCBase，用于管理视频解码的参数和操作。
     */
    class MediaVdec : public MediaInfVdec, public MediaParamCBase<MediaInfVdec::Param> {
    public:
        /**
         * @brief 构造函数，初始化视频解码实例。
         * 
         * @param chn 视频解码通道号。
         */
        explicit MediaVdec(int32_t chn) : MediaParamCBase(MediaModE::Vdec, chn) {}
    };

    /**
     * @brief 同步视频解码流类，用于处理视频解码流的同步操作。
     */
    class MediaVdecStream {
    public:
        /**
         * @brief 构造函数，初始化同步视频解码流实例。
         * 
         * @param chn 视频解码通道号。
         */
        explicit MediaVdecStream(int chn) : m_chn(chn) {}

        /**
         * @brief 创建视频解码流。
         * 
         * 该方法通过同步 RPC 请求服务器创建视频解码流，并初始化环形缓冲区。
         * @return 操作结果，成功返回 ErrCodeE::Success，失败返回相应错误码。
         */
        ErrCodeE Create();

        /**
         * @brief 销毁视频解码流。
         * 
         * 该方法用于释放视频解码流占用的资源。
         * @return 操作结果，成功返回 ErrCodeE::Success，失败返回相应错误码。
         */
        ErrCodeE Destroy();

        /**
         * @brief 推送一帧视频数据到解码流。
         * 
         * @param frame 待推送的视频帧数据。
         * @param timeoutMs 推送操作的超时时间，单位为毫秒。
         * @return 操作结果，成功返回 ErrCodeE::Success，失败返回相应错误码。
         */
        ErrCodeE PushFrame(MediaInfVdec::FrameData &frame, int32_t timeoutMs);

    private:
        int m_chn;                   //!< 视频解码通道号
        int m_key;                   //!< 环形缓冲区的键值
        MediaRingBufferWriter m_ring;//!< 环形缓冲区写入器
    };

    /**
     * @brief 异步视频解码流类，用于处理视频解码流的异步操作。
     */
    class MediaVdecStreamAsync {
    public:
        /**
         * @brief 构造函数，初始化异步视频解码流实例。
         * 
         * @param chn 视频解码通道号。
         */
        explicit MediaVdecStreamAsync(int chn) : m_chn(chn) {}

        /**
         * @brief 创建异步视频解码流。
         * 
         * 该方法绑定到指定的事件循环，创建异步视频解码流。
         * @param loop 要绑定的事件循环对象。
         * @return 操作结果，成功返回 ErrCodeE::Success，失败返回相应错误码。
         */
        ErrCodeE Create(EuvLoop &loop);

        /**
         * @brief 销毁异步视频解码流。
         * 
         * 该方法用于释放异步视频解码流占用的资源。
         * @return 操作结果，成功返回 ErrCodeE::Success，失败返回相应错误码。
         */
        ErrCodeE Destroy();

        /**
         * @brief 异步推送一帧视频数据到解码流。
         * 
         * @param frame 待推送的视频帧数据。
         * @return 操作结果，成功返回 ErrCodeE::Success，失败返回相应错误码。
         */
        ErrCodeE PushFrame(MediaInfVdec::FrameData &frame);

    private:
        MsgTopicPublisherAsync m_pub;//!< 异步消息主题发布器
        int m_chn;                   //!< 视频解码通道号
        int m_key;                   //!< 环形缓冲区的键值
        MediaRingBufferWriter m_ring;//!< 环形缓冲区写入器
    };
}// namespace Emx

#endif//EMX_MediaVdec_HPP
