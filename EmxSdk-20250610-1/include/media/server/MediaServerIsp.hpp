//
// Created by xiong on 2020/10/27.
//

#ifndef EMX_MediaServerIsp_HPP
#define EMX_MediaServerIsp_HPP

#include "MediaServerBase.hpp"
#include "core/euv/EuvTimer.hpp"
#include "media/common/MediaInfIsp.hpp"

namespace Emx {
    /** @addtogroup media-server
     *  @{
     */

    class MediaServerIspChn : public MediaInfIsp, public MediaServerModuleChnBase {
    public:
        explicit MediaServerIspChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Isp, chn) {}

        ~MediaServerIspChn() override = default;

        ErrCodeE LoadParam() override;

        ErrCodeE SaveParam() override;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }

        void *GetRunningParam() override { return &m_paramRunning; }

        int GetParamSize() override { return sizeof(Param); }

        ErrCodeE GetParamDefaultData(std::string &data) override;

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
        Param m_paramRunning;
    };

    class MediaServerIsp : public MediaServerModuleBase {
    public:
        MediaServerIsp() : MediaServerModuleBase(MediaModE::Isp, "isp") {}
    };
    /** @}*/
}// namespace Emx

#endif//EMX_MediaServerIsp_HPP
