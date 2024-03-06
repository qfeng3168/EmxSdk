//
// Created by xiong on 2021/8/5.
//

#ifndef EMX_MediaSystem_HPP
#define EMX_MediaSystem_HPP

#include "core/EmxTypeDef.hpp"

namespace Emx {

    //!@brief 控制媒体服务整体的启停
    class MediaSystem {
    public:
        /**
         * @brief 创建媒体
         * @return ErrCodeE
         */
        static ErrCodeE CreateMedia();

        /**
         * @brief 停止媒体
         * @return ErrCodeE
         */
        static ErrCodeE DestroyMedia();

        /**
         * @brief 切换场景
         * @return ErrCodeE
         */
        static ErrCodeE SwitchScene(const char *scene);
    };

}

#endif //EMX_MediaSystem_HPP
