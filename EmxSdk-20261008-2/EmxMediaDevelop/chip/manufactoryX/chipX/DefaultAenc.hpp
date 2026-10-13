//
// Created by xiong on 2020/11/19.
//

#ifndef EMX_DefaultAenc_HPP
#define EMX_DefaultAenc_HPP

#include "media/server/MediaServerAenc.hpp"
#include "EmxCore.hpp"

namespace Emx {
    class DefaultAenc : public MediaServerAencChn {
    public:
        DefaultAenc(int32_t chn) : MediaServerAencChn(chn){}

        virtual ~DefaultAenc() {};

        ErrCodeE OnCreate() override;

        void OnDestroy() override;

        ErrCodeE OnParamChanged() override;

        ErrCodeE GetOneFrame(FrameData &frame) override;

        void ReleaseOneFrame() override;

        ErrCodeE SetVolume(int32_t volume) override;

        ErrCodeE Mute(bool ena) override;

        ErrCodeE OnOffCtrl(bool onOff) override;
    protected:

        virtual ErrCodeE CreateAenc();

        virtual void DestroyAenc();

    protected:
        int32_t m_aiDev;
        int32_t m_aiChn;
        int32_t m_aencChn;
        int32_t m_vqeType;
        int m_gainMin;
        int m_gainMax;
        int m_aioSampleRate;
        int m_frameSize;
        Param *m_pParam;
        bool m_got;
    };
}

#endif //EMX_DefaultAenc_HPP
