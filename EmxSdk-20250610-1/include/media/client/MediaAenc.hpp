//
// Created by xiong on 2023/8/16.
//

#ifndef EMX_MediaAenc_HPP
#define EMX_MediaAenc_HPP

#include "media/common/MediaInfAenc.hpp"
#include "MediaParamCBase.hpp"

namespace Emx {
    //!@brief 音频编码
    class MediaAenc : public MediaInfAenc, public MediaParamCBase<MediaInfAenc::Param> {
    public:
        explicit MediaAenc(int32_t chn) : MediaParamCBase(MediaModE::Aenc, chn) {}

        ErrCodeE SetVolume(int32_t volume) override;

        ErrCodeE Mute(bool ena) override;

        ErrCodeE OnOffCtrl(bool onOff) override;
    };
}// namespace Emx
#endif//EMX_MediaAenc_HPP