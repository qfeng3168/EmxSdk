//
// Created by tfh on 2024/01/06.
//

#ifndef EMX_MediaInfVdec_HPP
#define EMX_MediaInfVdec_HPP

#include "media/common/MediaTypedef.hpp"

namespace Emx {

    class MediaInfVdec {
    public:
        //!@brief 视频解码持久化参数
        struct Param {
            VideoCodecE codec; //!<编码类型
            uint16_t width; //!<图像宽
            uint16_t height; //!<图像高
        };

        struct FrameData {
            EmxData data;
            uint64_t tsInUs;
        };
    };

}

#endif //EMX_MediaInfVdec_HPP