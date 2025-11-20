//
// Created by xiong on 2021/8/5.
//

#ifndef EMX_MediaSystem_HPP
#define EMX_MediaSystem_HPP

#include "core/EmxTypeDef.hpp"

namespace Emx {
    /** @addtogroup media-client
     *  @{
     */

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

        /**
         * @brief 临时切换场景
         * @param scene 场景名
         * @return ErrCodeE
         */
        static ErrCodeE SwitchSceneTemp(const char *scene);

        /**
         * @brief 获取当前场景
         * @param scene 场景名
         * @return ErrCodeE
         */
        static ErrCodeE GetScene(std::string &scene);

        /**
         * @brief 获取芯片信息
         * @param info [out] 返回芯片信息 json string，固定格式如下：
         * {
         *  "chipType": "SSC377QE",
         *  "chipId": "xxxxxxxx",
         *  "chipProductionDate": "2024-08-16",
         *  "chipProductionTime": "15:45:00",
         *  "chipSdkVersion": "V1.2.0"
         * }
         * @return ErrCodeE
         */
        static ErrCodeE GetChipInfo(std::string &info);
    };

    /** @}*/
}// namespace Emx

#endif//EMX_MediaSystem_HPP
