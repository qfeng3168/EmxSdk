//
// Created by xiong on 2020/10/27.
//

#ifndef EMX_MediaServerAenc_HPP
#define EMX_MediaServerAenc_HPP

#include "media/common/MediaInfAenc.hpp"
#include "MediaServerBase.hpp"
#include "core/euv/EuvWork.hpp"

namespace Emx {
    class AudioEncoder;

    class MediaServerAencChn : public MediaInfAenc, public MediaServerModuleChnBase {
    public:
        explicit MediaServerAencChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Aenc, chn), m_encoder(nullptr) {}

        ~MediaServerAencChn() override = default;

        ErrCodeE LoadParam() override;

        ErrCodeE SaveParam() override;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }

        int GetParamSize() override { return sizeof(Param); }

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

    private:

        virtual void OnStream(void *arg);

        virtual void OnStreamComplete(ErrCodeE e, void *arg);

        void PushAiFrame();

        void PushAencFrame();

    protected:
        Param m_param;
        Param m_paramNew;
    private:
        EuvWork m_work;
        FrameData m_frame;
        ErrCodeE m_getFrameErrCode;
        AudioEncoder *m_encoder;
    };

    class MediaServerAenc : public MediaServerModuleBase {
    public:
        MediaServerAenc() : MediaServerModuleBase(MediaModE::Aenc, "aenc") {}
    };
}

#endif //EMX_MediaServerAenc_HPP
