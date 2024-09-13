/*
* @Author: tangfahong
* @Date: 2023/9/12
*/

#ifndef EMX_MediaServerAov_HPP
#define EMX_MediaServerAov_HPP

#include "MediaServerBase.hpp"
#include "media/common/MediaInfAov.hpp"

namespace Emx {

    class MediaServerAovChn : public MediaInfAov, public MediaServerModuleChnBase {
    public:
        explicit MediaServerAovChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Aov, chn) {}

        ~MediaServerAovChn() override = default;

        ErrCodeE LoadParam() override;

        ErrCodeE SaveParam() override;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }
        
        void *GetRunningParam() override { return &m_paramRunning; }

        int GetParamSize() override { return sizeof(Param); }

        ErrCodeE GetParamDefaultData(std::string &data) override;

    protected:
        Param m_param;
        Param m_paramNew;
        Param m_paramRunning;
    };

    class MediaServerAov : public MediaServerModuleBase {
    public:
        MediaServerAov() : MediaServerModuleBase(MediaModE::Aov, "aov") {}
    };
}// namespace Emx

#endif//EMX_MediaServerAov_HPP
