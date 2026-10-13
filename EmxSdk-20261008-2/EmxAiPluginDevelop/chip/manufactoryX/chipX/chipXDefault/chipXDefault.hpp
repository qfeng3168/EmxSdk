//
// Created by xiong on 2022/4/27.
//

#ifndef EMX_chipXDefault_HPP
#define EMX_chipXDefault_HPP

#include "EmxCore.hpp"
#include "media/server/MediaServerAi.hpp"

namespace Emx {
    class chipXDefault : public MediaServerAiChn {
    public:
        chipXDefault(int32_t chn) : MediaServerAiChn(chn) {}

        virtual ~chipXDefault(){};

        /**
         * @brief 创建模块通道，需要自行实现配置文件和参数文件的加载
         * @return ErrCodeE
         */
        ErrCodeE OnCreate() override;

        /**
         * @brief 销毁模块通道
         */
        void OnDestroy() override;
    };
}// namespace Emx

#endif//EMX_chipXDefault_HPP
