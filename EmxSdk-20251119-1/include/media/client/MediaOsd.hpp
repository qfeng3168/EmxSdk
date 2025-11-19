//
// Created by xiong on 2021/8/9.
//

#ifndef EMX_MediaOsd_HPP
#define EMX_MediaOsd_HPP

#include "MediaParamCBase.hpp"
#include "media/common/MediaInfOsd.hpp"
#include <cstring>
#include <list>

namespace Emx {
    /** @addtogroup media-client
     *  @{
     */
    //!@brief OSD
    class MediaOsd : public MediaInfOsd, public MediaParamCBase<MediaInfOsd::Param> {
    public:
        explicit MediaOsd(int32_t chn) : MediaParamCBase(MediaModE::Osd, chn) {}

        ErrCodeE SetParam(const Param &param) override;

        ErrCodeE SetTmpParam(const Param &param) override;

        ErrCodeE GetParam(Param &param) override;

        ErrCodeE GetParamDefault(Param &param) override;

        ErrCodeE GetRunningParam(Param &param) override;

        ErrCodeE SetOsdContent(OsdContent *pInfo) override;
    };
    /** @}*/
}// namespace Emx
#endif//EMX_MediaOSD_HPP
