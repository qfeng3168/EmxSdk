//
// Created by xiong on 2021/8/10.
//

#ifndef EMX_DefaultOsd_HPP
#define EMX_DefaultOsd_HPP

#include "media/server/MediaServerOsd.hpp"
#include "EmxCore.hpp"

namespace Emx {
    class DefaultOsd : public MediaServerOsdChn {
    public:
        friend class DefaultVenc;

        DefaultOsd(int32_t chn) : MediaServerOsdChn(chn), m_osdIdx(0) {}

        virtual ~DefaultOsd() {};
    public:

        ErrCodeE OnCreate() override;

        void OnDestroy() override;

        Size GetChnPicSize() override;

        int RegionCreate(int x, int y, int w, int h, int layer, bool show) override;

        void RegionDestroy(int region) override;

        void SetBitMap(int region, uint8_t *pix, int w, int h) override;

        ErrCodeE ImageToCanvas(int region, const std::string &data) override;

    protected:
        int m_vpssGrp;
        int m_vpssChn;
        int m_chnWidth;
        int m_chnHeight;
        int m_osdIdx;
    };
}


#endif //EMX_DefaultOsd_HPP
