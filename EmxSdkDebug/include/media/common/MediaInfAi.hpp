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
        /*!
         * @brief 通用算法控制接口
         * @param in [in] 输入参数
         * @param out [out] 输出结果
         * return ErrCodeE
         */
        virtual ErrCodeE Ctrl(const Json::Value &in, Json::Value &out) = 0;
    };
}// namespace Emx

#endif//EMX_MediaInfAi_HPP