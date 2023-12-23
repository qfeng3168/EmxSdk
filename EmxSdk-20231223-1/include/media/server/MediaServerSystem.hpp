//
// Created by xiong on 2023/8/16.
//

#ifndef EMX_MediaServerSystem_HPP
#define EMX_MediaServerSystem_HPP

#include "media/common/MediaInfSystem.hpp"
#include "MediaServerBase.hpp"

namespace Emx {

    class MediaServerSystemChn : public MediaInfSystem, public MediaServerModuleChnBase {
    public:
        explicit MediaServerSystemChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::System, chn) {}

        ~MediaServerSystemChn() override = default;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }

        int GetParamSize() override { return sizeof(Param); }

        ErrCodeE OnParamChanged() override { return ErrCodeE::Success; }

    protected:
        Param m_param;
        Param m_paramNew;
    };

    class MediaServerSystem : public MediaServerModuleBase {
    public:
        MediaServerSystem() : MediaServerModuleBase(MediaModE::System, "system") {}
    };
}

#endif //EMX_MediaServerSystem_HPP
