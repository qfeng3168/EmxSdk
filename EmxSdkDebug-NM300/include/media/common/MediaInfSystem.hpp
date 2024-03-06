//
// Created by xiong on 2023/8/16.
//

#ifndef EMX_MediaInfSystem_HPP
#define EMX_MediaInfSystem_HPP

#include "media/common/MediaTypedef.hpp"

namespace Emx {
    //!@brief 系统接口
    class MediaInfSystem {
    public:
        //!@brief 系统持久化参数
        struct Param {
            bool reserved; //!<无
        };
    };
}

#endif //EMX_MediaInfSystem_HPP