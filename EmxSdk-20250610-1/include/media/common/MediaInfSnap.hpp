//
// Created by xiong on 2023/8/22.
//

#ifndef EMX_MediaInfSnap_HPP
#define EMX_MediaInfSnap_HPP

#include "media/common/MediaTypedef.hpp"

namespace Emx {
    /** @addtogroup media-common
     *  @{
     */

    class MediaInfSnap {
    public:
        //!@brief 缩略图持久化参数
        struct Param {
            bool reserved; //!<无
        };

        /**
         * @brief 触发缩略图编码，调用此接口后，媒体服务会编码一帧缩略图，
         *        并通过MediaFrame(ringbuffer)广播出去.
         *        注意调用此接口前需要保证已经在监听MediaFrame
         * @param timeoutMs [in] 超时时间
         * @return ErrCodeE
         */
        virtual ErrCodeE Trigger(int32_t timeoutMs) = 0;
    };
    /** @}*/
}

#endif //EMX_MediaInfSnap_HPP