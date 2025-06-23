//
// Created by xiong on 2020/10/27.
//

#ifndef EMX_MediaServerAenc_HPP
#define EMX_MediaServerAenc_HPP

#include "MediaServerBase.hpp"
#include "core/euv/EuvWork.hpp"
#include "media/common/MediaInfAenc.hpp"

namespace Emx {
    /** @addtogroup media-server
     *  @{
     */
    class AudioEncoder;
    class TimestampPtsToUtc;

    class MediaServerAencChn : public MediaInfAenc, public MediaServerModuleChnBase {
    public:
        explicit MediaServerAencChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Aenc, chn), m_encoder(nullptr) {}

        ~MediaServerAencChn() override = default;

        ErrCodeE LoadParam() override;

        ErrCodeE SaveParam() override;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }

        void *GetRunningParam() override { return &m_paramRunning; }

        int GetParamSize() override { return sizeof(Param); }

        ErrCodeE GetParamDefaultData(std::string &data) override;

        bool ParamValid() override;

        ErrCodeE OnRecvReqChn(uint32_t method, const char *data, int32_t size, std::string &out) override;

    public:
        /**
         * @brief 开始获取原始音频流，调用后GetOneFrame会被不停触发
         */
        virtual void StartStream();

        /**
         * @brief 停止获取原始音频流
         */
        virtual void StopStream();

        struct FrameData {
            EmxData data;
            uint64_t tsInUs;
        };

        /**
         * @brief 获取一帧PCM音频
         * @param frame [out] 将获取到的音频数据放入data中
         * @return ErrCodeE
         */
        virtual ErrCodeE GetOneFrame(FrameData &frame) = 0;

        /**
         * @brief 释放一帧编码图像，仅当GetOneFrame返回成功时此函数才会被执行
         */
        virtual void ReleaseOneFrame() = 0;

        virtual ErrCodeE OnOffCtrl(bool onOff);    

    private:
        virtual void OnStream(void *arg);

        virtual void OnStreamComplete(ErrCodeE e, void *arg);

        void PushAiFrame();

        void PushAencFrame();

    protected:
        Param m_param;
        Param m_paramNew;
        Param m_paramRunning;
        TimestampPtsToUtc *m_ptsToUtc;

    private:
        EuvWork m_work;
        FrameData m_frame;
        ErrCodeE m_getFrameErrCode;
        AudioEncoder *m_encoder;
        AudioCodecE m_codec;
        bool m_codecChange;
    };

    class MediaServerAenc : public MediaServerModuleBase {
    public:
        MediaServerAenc() : MediaServerModuleBase(MediaModE::Aenc, "aenc") {}
    };
    /** @}*/
}// namespace Emx

#endif//EMX_MediaServerAenc_HPP
