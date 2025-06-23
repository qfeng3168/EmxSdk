//
// Created by tfh on 2024/01/06.
//

#ifndef EMX_MediaInfVo_HPP
#define EMX_MediaInfVo_HPP

#include "media/common/MediaTypedef.hpp"

namespace Emx {
    /** @addtogroup media-common
     *  @{
     */

    class MediaInfVo {
    public:
        //!@brief 视频输出持久化参数
        struct Param {
            bool show;      //!< 是否显示
            bool loopVi;    //!< 是否本地回环显示摄像头数据
        };
    };
    /** @}*/
}

#endif //EMX_MediaInfVo_HPP