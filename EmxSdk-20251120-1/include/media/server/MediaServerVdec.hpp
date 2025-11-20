//
// Created by tfh on 2024/01/06.
//

#ifndef EMX_MediaServerVdec_HPP
#define EMX_MediaServerVdec_HPP

#include "EmxCore.hpp"
#include "MediaServerBase.hpp"
#include "media/common/MediaInfVdec.hpp"
#include "media/common/MediaRingBuffer.hpp"

namespace Emx {
    /** @addtogroup media-server
     *  @{
     */

    class MediaServerVdecChn : public MediaInfVdec, public MediaServerModuleChnBase {
    public:
        explicit MediaServerVdecChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Vdec, chn) {}

        ~MediaServerVdecChn() override = default;

        ErrCodeE LoadParam() override;

        ErrCodeE SaveParam() override;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }

        void *GetRunningParam() override { return &m_paramRunning; }

        int GetParamSize() override { return sizeof(Param); }

        ErrCodeE GetParamDefaultData(std::string &data) override;

        ErrCodeE OnRecvReqChn(uint32_t method, const char *data, int32_t size, std::string &out) override;

        ErrCodeE OnServerCreate() override;

        void OnServerDestroy() override;

    protected:
        struct FrameData {
            char *data;
            uint32_t size;
            uint64_t tsInUs;
        };
        /**
         * @brief 发送视频流到解码器
         * @param data 编码视频数据 一般为264 265 jpeg
         * @return ErrCodeE
        */
        virtual ErrCodeE SendFrame(FrameData &data) = 0;

        /**
         * @brief 发送视频流到解码器并且返回解码后数据
         * @param data 编码视频数据 一般为264 265 jpeg
         * @param raw 视频解码后的数据 一般为rgb888 yuv420
         * @return ErrCodeE
        */
        virtual ErrCodeE SendFrameDecode(FrameData &data, FrameData &raw);

        /**
         * @brief 获取解码器输出数据大小
         * @return uint32_t
        */
        virtual uint32_t GetVdecRawSize();

        /**
         * @brief 将解码后的数据推送出去
         * @return ErrCodeE
        */
        ErrCodeE PushVdecData(FrameData &fData);


    protected:
        Param m_param;       //!< 当前参数
        Param m_paramNew;    //!< 新参数
        Param m_paramRunning;//!< 运行参数

    public:
        MsgTopicSubscriberAsync m_sub;    //!< 消息主题订阅者异步
        MediaRingBufferReader m_ring;     //!< 环形缓冲区读取器
        MediaRingBufferWriter m_ringWrite;//!< 环形缓冲区写入器

    private:
        int m_key;            //!< 环形缓冲区键值
        int m_ringBufSize;    //!< 环形缓冲区大小
        int m_ringReadBufSize;//!< 环形缓冲区读取大小
        int m_ringBusy;       //!< 环形缓冲区是否繁忙
    };

    class MediaServerVdec : public MediaServerModuleBase {
    public:
        MediaServerVdec() : MediaServerModuleBase(MediaModE::Vdec, "vdec") {}
    };
    /** @}*/
}// namespace Emx

#endif//EMX_MediaServerVdec_HPP
