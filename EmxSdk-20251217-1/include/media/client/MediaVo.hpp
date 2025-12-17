//
// Created by tfh on 2024/01/06.
//

#ifndef EMX_MediaVo_HPP
#define EMX_MediaVo_HPP

#include "media/common/MediaInfVo.hpp"
#include "MediaParamCBase.hpp"

namespace Emx {
    /** @addtogroup media-client
     *  @{
     */
    //!@brief 视频输出
    class MediaVo : public MediaInfVo, public MediaParamCBase<MediaInfVo::Param> {
    public:
        explicit MediaVo(int32_t chn) : MediaParamCBase(MediaModE::Vo, chn) {}

    };
    /** @}*/
}

#endif //EMX_MediaVo_HPP
