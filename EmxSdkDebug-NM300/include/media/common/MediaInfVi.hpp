//
// Created by xiong on 2023/8/22.
//

#ifndef EMX_MediaViInf_HPP
#define EMX_MediaViInf_HPP

#include "media/common/MediaTypedef.hpp"

namespace Emx {
    //!@brief 音频编码接口
    class MediaInfVi {
    public:
        //!@brief 视频输入持久化参数
        struct Param {
            bool flip;  //!<是否上下翻转
            bool mirror;//!<是否左右镜像
        };
        /**
         * @brief 设置视频遮挡状态
         * @param enable [bool] 遮挡使能
         * @return ErrCodeE
         */
        virtual ErrCodeE SetHideStatus(bool enable) = 0;
    };
}// namespace Emx

#endif//EMX_MediaViInf_HPP