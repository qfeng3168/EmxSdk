//
// Created by xiong on 2020/10/27.
//

#ifndef EMX_MediaServerSnap_HPP
#define EMX_MediaServerSnap_HPP

#include "media/common/MediaInfSnap.hpp"
#include "MediaServerBase.hpp"
#include "EmxCore.hpp"

namespace Emx {

    class MediaServerSnapChn : public MediaInfSnap, public MediaServerModuleChnBase {
    public:
        explicit MediaServerSnapChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Snap, chn) {}

        ~MediaServerSnapChn() override = default;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }

        int GetParamSize() override { return sizeof(Param); }

        ErrCodeE OnRecvReqChn(uint32_t method, const char *data, int32_t size, std::string &out) override;

        ErrCodeE OnParamChanged() override { return ErrCodeE::Success; }

        ErrCodeE OnServerCreate() override;

        void OnServerDestroy() override;

        ErrCodeE Trigger(int32_t timeoutMs) override;

    protected:

        /**
         * @brief 获取一帧抓拍图像
         * @param data [out] 将获取到的图像数据放入data中，vector类型可支持非连续内存数据
         * @return ErrCodeE
         */
        virtual ErrCodeE GetOneSnap(std::vector<EmxData> &data) = 0;

        /**
         * @brief 释放一帧抓拍图像，仅当GetOneSnap返回成功时此函数才会被执行
         */
        virtual void ReleaseOneSnap() = 0;

    private:

        void OnTrigger(void *arg);

        void OnTriggerComplete(ErrCodeE e, void *arg);

    protected:
        Param m_param;
        Param m_paramNew;
    private:
        bool m_isSnapping;
        EuvWork m_work;
        std::vector<EmxData> m_data;
        ErrCodeE m_getSnapErrCode;
    };

    class MediaServerSnap : public MediaServerModuleBase {
    public:
        MediaServerSnap() : MediaServerModuleBase(MediaModE::Snap, "snap") {}
    };
}

#endif //EMX_MediaServerSnap_HPP
