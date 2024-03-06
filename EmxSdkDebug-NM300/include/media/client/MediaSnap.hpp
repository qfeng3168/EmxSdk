//
// Created by xiong on 2021/5/28.
//

#ifndef EMX_MediaSnap_HPP
#define EMX_MediaSnap_HPP

#include "media/common/MediaInfSnap.hpp"
#include "MediaParamCBase.hpp"

namespace Emx {
    //!@brief 图像抓拍
    class MediaSnap : public MediaInfSnap, public MediaParamCBase<MediaInfSnap::Param> {
    public:
        explicit MediaSnap(int32_t chn) : MediaParamCBase(MediaModE::Snap, chn) {}

        ErrCodeE Trigger(int32_t timeoutMs) override;
    };
}
#endif //EMX_MediaSnap_HPP

