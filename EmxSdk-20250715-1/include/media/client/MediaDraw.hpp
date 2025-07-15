/*
* @Author: tangfahong
* @Date: 2024/3/22
*/

#ifndef EMX_MediaDraw_HPP
#define EMX_MediaDraw_HPP

#include "MediaParamCBase.hpp"
#include "media/common/MediaInfDraw.hpp"
#include <cstring>
#include <list>

namespace Emx {
    /** @addtogroup media-client
     *  @{
     */
    //!@brief Draw
    class MediaDraw : public MediaInfDraw, public MediaParamCBase<MediaInfDraw::Param> {
    public:
        explicit MediaDraw(int32_t chn) : MediaParamCBase(MediaModE::Draw, chn) {}

        ErrCodeE SetParam(const Param &param) override;

        ErrCodeE GetParam(Param &param) override;

        ErrCodeE GetParamDefault(Param &param) override;

        ErrCodeE SetTmpParam(const Param &param) override;

        ErrCodeE GetRunningParam(Param &param) override;
    };
    /** @}*/
}// namespace Emx
#endif//EMX_MediaOSD_HPP
