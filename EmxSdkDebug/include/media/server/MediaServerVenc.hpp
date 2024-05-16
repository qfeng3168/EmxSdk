//
// Created by xiong on 2020/10/27.
//

#ifndef EMX_MediaServerVenc_HPP
#define EMX_MediaServerVenc_HPP

#include "EmxCore.hpp"
#include "MediaServerBase.hpp"
#include "media/common/MediaInfVenc.hpp"

namespace Emx {

    class MediaServerVencChn : public MediaInfVenc, public MediaServerModuleChnBase {
    public:
        explicit MediaServerVencChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Venc, chn), m_seq(0) {}

        ~MediaServerVencChn() override = default;

        ErrCodeE LoadParam() override;

        ErrCodeE SaveParam() override;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }
        
        void *GetRunningParam() override { return &m_paramRunning; }

        int GetParamSize() override { return sizeof(Param); }

        ErrCodeE GetParamDefaultData(std::string &data) override;

        bool ParamValid() override;

        ErrCodeE OnRecvReqChn(uint32_t method, const char *data, int32_t size, std::string &out) override;

    protected:
        /**
         * @brief 开始获取编码视频流，调用后GetOneFrame会被不停触发
         */
        virtual void StartStream();

        /**
         * @brief 停止获取编码视频流
         */
        virtual void StopStream();

        struct FrameData {
            std::vector<EmxData> data;
            uint64_t tsInUs;
            bool isKeyFrame;
            bool isParamChanged;
        };

        /**
         * @brief 获取一帧编码图像
         * @param frame [out] 将获取到的图像数据放入data中，vector类型可支持非连续内存数据
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

    protected:
        Param m_param;
        Param m_paramNew;
        Param m_paramRunning;

    private:
        EuvWork m_work;
        FrameData m_frame;
        ErrCodeE m_getFrameErrCode;
        uint32_t m_seq;
    };

    class MediaServerVenc : public MediaServerModuleBase {
    public:
        MediaServerVenc() : MediaServerModuleBase(MediaModE::Venc, "venc") {}
    };
}// namespace Emx

#endif//EMX_MediaServerVenc_HPP
