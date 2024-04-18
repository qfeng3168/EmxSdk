//
// Created by xiong on 2023/9/12.
//

#ifndef EMX_MediaInfAi_HPP
#define EMX_MediaInfAi_HPP

#include "media/common/MediaTypedef.hpp"
namespace Emx {
    class MediaInfAi {
    public:
        /**
         * @brief 获取Ai插件版本
         * @param version 版本号
         * @return ErrCodeE
         */
        virtual ErrCodeE GetVersion(std::string &version) = 0;
    };
}// namespace Emx

#endif//EMX_MediaInfAi_HPP