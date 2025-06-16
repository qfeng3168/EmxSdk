/*
* @Author: tangfahong
* @Date: 2023/9/12
*/

#ifndef EMX_MediaServerAov_HPP
#define EMX_MediaServerAov_HPP

#include "MediaServerBase.hpp"
#include "media/common/MediaInfAov.hpp"

namespace Emx {

    class MediaServerAovChn : public MediaInfAov, public MediaServerModuleChnBase {
    public:
        explicit MediaServerAovChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Aov, chn) {}

        ~MediaServerAovChn() override = default;

        ErrCodeE LoadParam() override;

        ErrCodeE SaveParam() override;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }

        void *GetRunningParam() override { return &m_paramRunning; }

        int GetParamSize() override { return sizeof(Param); }

        ErrCodeE GetParamDefaultData(std::string &data) override;

        ErrCodeE OnRecvReqChn(uint32_t method, const char *data, int32_t size, std::string &out) override;

        /**
         * @brief 推送ai触发消息
         * @param trigger [in] 是否触发
         * @return ErrCodeE
         */
        virtual ErrCodeE PushAiTrigger(bool trigger);
    protected:
        /**
         * @brief 更新aov状态
         * 需要mediaServer实现
         * @param type [in] 状态类型
         * @return ErrCodeE
         */
        virtual ErrCodeE UpdateStatus(AovStatusTypeE type);

        /**
         * @brief 推送aov消息
         * 需要mediaServer调用
         * @param type [in] 推送类型
         * @param data [in] 数据
         * @param dataSize [in] 数据大小
         * @return ErrCodeE
         */
        virtual ErrCodeE PushMessage(AovPushTypeE type,char *data,int dataSize);

    protected:
        Param m_param;
        Param m_paramNew;
        Param m_paramRunning;
    };

    class MediaServerAov : public MediaServerModuleBase {
    public:
        MediaServerAov() : MediaServerModuleBase(MediaModE::Aov, "aov") {}
    };
}// namespace Emx

#endif//EMX_MediaServerAov_HPP
