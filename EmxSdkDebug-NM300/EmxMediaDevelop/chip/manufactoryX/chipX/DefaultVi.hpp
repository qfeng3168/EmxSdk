//
// Created by xiong on 2023/9/6.
//

#ifndef EMX_DefaultVi_HPP
#define EMX_DefaultVi_HPP

#include "EmxCore.hpp"
#include "media/server/MediaServerVi.hpp"

namespace Emx {

    class DefaultVi : public MediaServerViChn {
    public:
        DefaultVi(int32_t chn) : MediaServerViChn(chn) {}

        virtual ~DefaultVi(){};

        ErrCodeE OnCreate() override;

        void OnDestroy() override;

        ErrCodeE OnParamChanged() override;

    private:
        ErrCodeE SetMirrorFlip(Param &param);
        
        ErrCodeE SetHideStatus(bool enable);
    protected:
        int m_pipe;
        int m_viChn;
        bool m_originFlip;
        bool m_originMirror;
    };
}// namespace Emx

#endif//EMX_DefaultVi_HPP
