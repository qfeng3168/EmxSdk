//
// Created by xiong on 2023/8/22.
//

#ifndef EMX_MediaInfQrScan_HPP
#define EMX_MediaInfQrScan_HPP

#include "media/common/MediaTypedef.hpp"

namespace Emx {
    /** @addtogroup media-common
     *  @{
     */

    class MediaInfQrScan {
    public:
        //!@brief 扫码持久化参数
        struct Param {
            bool reserved; //!<无
        };

        /**
         * @brief 启动扫码
         * @return ErrCodeE
         */
        virtual ErrCodeE Start() = 0;

        /**
         * @brief 结束扫码
         * @return ErrCodeE
         */
        virtual ErrCodeE Stop() = 0;
    };

    /** @}*/
}

#endif //EMX_MediaInfQrScan_HPP