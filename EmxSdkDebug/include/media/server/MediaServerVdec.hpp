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

    class MediaServerVdecChn : public MediaInfVdec, public MediaServerModuleChnBase {
    public:
        explicit MediaServerVdecChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Vdec, chn) {}

        ~MediaServerVdecChn() override = default;

        ErrCodeE LoadParam() override;

        ErrCodeE SaveParam() override;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }

        int GetParamSize() override { return sizeof(Param); }

        ErrCodeE GetParamDefaultData(std::string &data) override;

        ErrCodeE OnRecvReqChn(uint32_t method, const char *data, int32_t size, std::string &out) override;

        ErrCodeE OnServerCreate() override;

        void OnServerDestroy() override;

    protected:
        struct FrameData {
            uint8_t *data;
            uint32_t size;
            uint64_t tsInUs;
        };
        /**
         * @brief 发送视频流到解码器并且返回解码后数据
         * @param data 编码视频数据 一般为264 265 jpeg
         * @return ErrCodeE
        */
        virtual ErrCodeE SendFrame(FrameData &data) = 0;

    protected:
        Param m_param;
        Param m_paramNew;

    public:
        MsgTopicSubscriberAsync m_sub;
        MediaRingBufferReader m_ring;

    private:
        int m_key;
        int m_ringBufSize;
        int m_ringBusy;
    };

    class MediaServerVdec : public MediaServerModuleBase {
    public:
        MediaServerVdec() : MediaServerModuleBase(MediaModE::Vdec, "vdec") {}
    };
}// namespace Emx

#endif//EMX_MediaServerVdec_HPP
