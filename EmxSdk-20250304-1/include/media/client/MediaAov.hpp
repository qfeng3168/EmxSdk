//
// Created by tfh on 2024/09/02.
//

#ifndef EMX_MediaAov_HPP
#define EMX_MediaAov_HPP

#include "media/common/MediaInfAov.hpp"
#include "MediaParamCBase.hpp"

namespace Emx {
    //!@brief 视频输出
    class MediaAov : public MediaInfAov, public MediaParamCBase<MediaInfAov::Param> {
    public:
        explicit MediaAov(int32_t chn) : MediaParamCBase(MediaModE::Aov, chn) {}
    };
}// namespace Emx

#endif//EMX_MediaAov_HPP
