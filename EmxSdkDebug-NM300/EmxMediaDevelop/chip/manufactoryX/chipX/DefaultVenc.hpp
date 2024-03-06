/**
 * @author xiaoxiong
 * @brief
 * @date 2022-03-24
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef EMX_DefaultVenc_HPP
#define EMX_DefaultVenc_HPP

#include "media/server/MediaServerVenc.hpp"
#include "EmxCore.hpp"

namespace Emx {

    class DefaultVenc : public MediaServerVencChn {
    public:
        DefaultVenc(int32_t chn)
                : MediaServerVencChn(chn), m_paramChange(false), m_seq(0) {}

        virtual ~DefaultVenc() {};

        ErrCodeE OnCreate() override;

        void OnDestroy() override;

        ErrCodeE OnParamChanged() override;

        ErrCodeE FlushIDR() override;

    protected:
        ErrCodeE GetOneFrame(FrameData &frame) override;

        void ReleaseOneFrame() override;

    protected:

        virtual ErrCodeE CreateVenc();

        virtual void DestroyVenc();

    protected:
        Param *m_pParam;
        int m_vpssGrp;
        int m_vpssChn;
        int m_vencChn;
        bool m_paramChange;
        uint32_t m_seq;
        void *m_stream;
    };
}// namespace Emx

#endif// EMX_DefaultVenc_HPP
