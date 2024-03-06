//
// Created by xiong on 2020/10/27.
//

#ifndef EMX_MediaServerIsp_HPP
#define EMX_MediaServerIsp_HPP

#include "media/common/MediaInfIsp.hpp"
#include "MediaServerBase.hpp"
#include "core/euv/EuvTimer.hpp"

namespace Emx {

    class MediaServerIspChn : public MediaInfIsp, public MediaServerModuleChnBase {
    public:
        explicit MediaServerIspChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Isp, chn) {}

        ~MediaServerIspChn() override = default;

        ErrCodeE LoadParam() override;

        ErrCodeE SaveParam() override;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }

        int GetParamSize() override { return sizeof(Param); }

        bool ParamValid() override;

        ErrCodeE OnRecvReqChn(uint32_t method, const char *data, int32_t size, std::string &out) override;

        ErrCodeE OnServerCreate() override;

        void OnServerDestroy() override;

    protected:

        /**
         * @brief 定时触发收集isp信息
         * @param info [out] isp信息
         * @return ErrCodeE
         */
        virtual ErrCodeE Collection(Info &info) = 0;

    private:
        EuvTimer m_timer;
        int m_infoInvMs;

    protected:
        Param m_param;
        Param m_paramNew;
    };

    class MediaServerIsp : public MediaServerModuleBase {
    public:
        MediaServerIsp() : MediaServerModuleBase(MediaModE::Isp, "isp") {}
    };
}

#endif //EMX_MediaServerIsp_HPP
