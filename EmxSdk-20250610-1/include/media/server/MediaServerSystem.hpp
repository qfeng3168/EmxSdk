//
// Created by xiong on 2023/8/16.
//

#ifndef EMX_MediaServerSystem_HPP
#define EMX_MediaServerSystem_HPP

#include "media/common/MediaInfSystem.hpp"
#include "MediaServerBase.hpp"

namespace Emx {

    class MediaServerSystemChn : public MediaInfSystem, public MediaServerModuleChnBase {
    public:
        explicit MediaServerSystemChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::System, chn) {}

        ~MediaServerSystemChn() override = default;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }

        void *GetRunningParam() override { return &m_paramRunning; }

        int GetParamSize() override { return sizeof(Param); }

        ErrCodeE OnParamChanged() override { return ErrCodeE::Success; }

        ErrCodeE OnRecvReqChn(uint32_t method, const char *data, int32_t size, std::string &out) override;

        /**
         * @brief 获取芯片信息
         * @param info [out] 返回芯片信息 json string
         * @return ErrCodeE
         */
        ErrCodeE GetChipInfo(std::string &info) override;
    protected:
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
        virtual ErrCodeE OnGetChipInfo(Json::Value &info) = 0;

    protected:
        Param m_param;
        Param m_paramNew;
        Param m_paramRunning;
    };

    class MediaServerSystem : public MediaServerModuleBase {
    public:
        MediaServerSystem() : MediaServerModuleBase(MediaModE::System, "system") {}
    };
}// namespace Emx

#endif//EMX_MediaServerSystem_HPP
