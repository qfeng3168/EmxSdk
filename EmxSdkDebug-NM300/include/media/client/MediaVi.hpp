/*
* @Author: xiong
* @Date: 2023/4/3
*/

#ifndef EMX_MediaVi_HPP
#define EMX_MediaVi_HPP

#include "media/common/MediaInfVi.hpp"
#include "MediaParamCBase.hpp"

namespace Emx {
    //!@brief 视频输入
    class MediaVi : public MediaInfVi, public MediaParamCBase<MediaInfVi::Param> {
    public:
        explicit MediaVi(int32_t chn) : MediaParamCBase(MediaModE::Vi, chn) {}
    };
}
#endif //EMX_MediaVi_HPP
