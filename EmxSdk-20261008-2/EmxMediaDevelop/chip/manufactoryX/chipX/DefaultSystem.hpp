//
// Created by xiong on 2020/10/27.
//

#ifndef EMX_DefaultSystem_HPP
#define EMX_DefaultSystem_HPP

#include "media/server/MediaServerSystem.hpp"
#include "EmxCore.hpp"

namespace Emx {

    class DefaultSystem : public MediaServerSystemChn {
    public:
        DefaultSystem(int32_t chn) : MediaServerSystemChn(chn) {}

        virtual ~DefaultSystem() {}

        ErrCodeE OnCreate() override;

        void OnDestroy() override;
         /**
         * @brief 获取芯片信息
         * @param info [out] 返回芯片信息 json 对象，固定格式如下：
         * {
         *  "chipType": "SSC377QE",
         *  "chipId": "xxxxxxxx",
         *  "chipProductionDate": "2024-08-16",
         *  "chipProductionTime": "15:45:00",
         *  "chipSdkVersion": "V1.2.0"
         * }
         * @return ErrCodeE
         */
        ErrCodeE OnGetChipInfo(Json::Value &info) override;
    };
}


#endif //EMX_DefaultSystem_HPP
