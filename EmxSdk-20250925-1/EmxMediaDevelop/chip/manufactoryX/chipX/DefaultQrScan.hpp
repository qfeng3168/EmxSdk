//
// Created by xiong on 2023/9/6.
//

#ifndef EMX_DefaultQrScan_HPP
#define EMX_DefaultQrScan_HPP

#include "media/server/MediaServerQrScan.hpp"
#include "EmxCore.hpp"

namespace Emx {

    class DefaultQrScan : public MediaServerQrScanChn {
    public:
        DefaultQrScan(int32_t chn) : MediaServerQrScanChn(chn) {}

        virtual ~DefaultQrScan() {};

    protected:
        ErrCodeE OnCreate() override;

        void OnDestroy() override;

        Size GetChnImgSize() override;

        ErrCodeE GetOneImg(uint8_t **dataY) override;

        void ReleaseOneImg(uint8_t *dataY) override;

    private:
        int m_vpssGrp;
        int m_vpssChn;
        int m_mem;
    };

}

#endif //EMX_DefaultQrScan_HPP
