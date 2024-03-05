//
// Created by xiong on 2020/11/23.
//

#ifndef EMX_DefaultAdec_HPP
#define EMX_DefaultAdec_HPP

#include "media/server/MediaServerAdec.hpp"
#include "EmxCore.hpp"

namespace Emx {
    class DefaultAdec : public MediaServerAdecChn {
    public:
        DefaultAdec(int32_t chn) :
                MediaServerAdecChn(chn),
                m_aoDev(0),
                m_aoChn(0),
                m_adecChn(0) {}

        virtual ~DefaultAdec() {};

        ErrCodeE OnCreate() override;

        void OnDestroy() override;

        ErrCodeE OnParamChanged() override;

        ErrCodeE SetVolume(int32_t volume) override;

        PlayStatusE GetPlayStatus() override;

        void SendToAo(const int16_t *pcm, int32_t pktNum) override;

    protected:

        virtual ErrCodeE CreateAdec();

        virtual void DestroyAdec();

    protected:
        int32_t m_aoDev;
        int32_t m_aoChn;
        int32_t m_adecChn;
        int m_gainMin;
        int m_gainMax;
        int m_aioSampleRate;
        int m_frameSize;
        Param *m_pParam;

    };
}
#endif //EMX_DefaultAdec_HPP
