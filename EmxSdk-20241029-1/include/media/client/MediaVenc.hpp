//
// Created by xiong on 2020/6/15.
//

#ifndef EMX_MediaVenc_HPP
#define EMX_MediaVenc_HPP

#include "media/common/MediaInfVenc.hpp"
#include "MediaParamCBase.hpp"

namespace Emx {
    //!@brief 视频编码
    class MediaVenc : public MediaInfVenc, public MediaParamCBase<MediaInfVenc::Param> {
    public:
        explicit MediaVenc(int32_t chn) : MediaParamCBase(MediaModE::Venc, chn) {}

        ErrCodeE FlushIDR() override;
    };
}

#endif //EMX_MediaVenc_HPP
