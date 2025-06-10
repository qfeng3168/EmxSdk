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
            bool reserved;//!<无
        };
        /**
         * @brief 获取芯片信息
         * @param info [out] 返回芯片信息 json string,固定格式如下：
         * {
         *  "chipType": "SSC377QE",
         *  "chipId": "xxxxxxxx",
         *  "chipProductionDate": "2024-08-16",
         *  "chipProductionTime": "15:45:00",
         *  "chipSdkVersion": "V1.2.0"
         * }
         * @return ErrCodeE
         */
        virtual ErrCodeE GetChipInfo(std::string &info) = 0;
    };
}// namespace Emx

#endif//EMX_MediaInfSystem_HPP