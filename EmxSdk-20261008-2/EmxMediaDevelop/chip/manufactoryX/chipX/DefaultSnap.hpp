//
// Created by xiong on 2023/9/6.
//

#ifndef EMX_DefaultSnap_HPP
#define EMX_DefaultSnap_HPP

#include "media/server/MediaServerSnap.hpp"
#include "EmxCore.hpp"

namespace Emx {
    class DefaultSnap : public MediaServerSnapChn {
    public:
        DefaultSnap(int32_t chn) : MediaServerSnapChn(chn) {}

        virtual ~DefaultSnap() {};
    protected:

        ErrCodeE OnCreate() override;

        void OnDestroy() override;

        ErrCodeE GetOneSnap(std::vector <EmxData> &data) override;

        void ReleaseOneSnap() override;

    protected:
        int32_t m_vpssGrp;
        int32_t m_vpssChn;
        int32_t m_vencChn;
    };

}

#endif //EMX_DefaultSnap_HPP
