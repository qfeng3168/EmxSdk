//
// Created by tfh on 2024/01/06.
//

#ifndef EMX_MediaServerVo_HPP
#define EMX_MediaServerVo_HPP

#include "EmxCore.hpp"
#include "MediaServerBase.hpp"
#include "media/common/MediaInfVo.hpp"

namespace Emx {
    /** @addtogroup media-server
     *  @{
     */

    class MediaServerVoChn : public MediaInfVo, public MediaServerModuleChnBase {
    public:
        explicit MediaServerVoChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Vo, chn) {}

        ~MediaServerVoChn() override = default;

        ErrCodeE LoadParam() override;

        ErrCodeE SaveParam() override;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }
        
        void *GetRunningParam() override { return &m_paramRunning; }

        int GetParamSize() override { return sizeof(Param); }

        ErrCodeE GetParamDefaultData(std::string &data) override;

        ErrCodeE OnRecvReqChn(uint32_t method, const char *data, int32_t size, std::string &out) override;

    protected:
        /**
         * @brief 开始将vi视频发送到vo
         */
        virtual void StartLoopViSendVo();

        /**
         * @brief 停止将vi视频发送到vo
         */
        virtual void StopLoopViSendVo();

        /**
         * @brief 将vi视频发送到vo
         * @return ErrCodeE
         */
        virtual ErrCodeE runOneViSendVo() = 0;

    private:
        virtual void OnViSendVo(void *arg);

        virtual void OnViSendVoComplete(ErrCodeE e, void *arg);

    protected:
        Param m_param;
        Param m_paramNew;
        Param m_paramRunning;

    private:
        EuvWork m_work;
    };

    class MediaServerVo : public MediaServerModuleBase {
    public:
        MediaServerVo() : MediaServerModuleBase(MediaModE::Vo, "vo") {}
    };
    /** @}*/
}// namespace Emx

#endif//EMX_MediaServerVo_HPP
