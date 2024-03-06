/*
* @Author: xiong
* @Date: 2023/4/3
*/

#ifndef EMX_MediaServerVi_HPP
#define EMX_MediaServerVi_HPP

#include "media/common/MediaInfVi.hpp"
#include "MediaServerBase.hpp"

namespace Emx {

    class MediaServerViChn : public MediaInfVi, public MediaServerModuleChnBase {
    public:
        explicit MediaServerViChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Vi, chn) {}

        ~MediaServerViChn() override = default;

        ErrCodeE LoadParam() override;

        ErrCodeE SaveParam() override;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }

        int GetParamSize() override { return sizeof(Param); }

    protected:
        Param m_param;
        Param m_paramNew;
    };

    class MediaServerVi : public MediaServerModuleBase {
    public:
        MediaServerVi() : MediaServerModuleBase(MediaModE::Vi, "vi") {}
    };
}

#endif //EMX_MediaServerVi_HPP
