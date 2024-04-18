//
// Created by xiong on 2020/10/27.
//

#ifndef EMX_MediaServerAi_HPP
#define EMX_MediaServerAi_HPP

#include "MediaServerBase.hpp"
#include "core/euv/EuvWork.hpp"
#include "media/common/MediaInfAi.hpp"
#include "media/common/MediaInfAiData.hpp"
#include "media/common/MediaRingBuffer.hpp"
#include "media/server/EmxAiPlugin.h"
namespace Emx {
    class MediaAiJob {
    public:
        using OnCallBack = std::function<void(void *arg)>;

        void Create(OnCallBack onWork, OnCallBack onWorkComplete, void *arg = nullptr);

        virtual void Destroy();

        static ErrCodeE PushAiData(MediaInfAiData *data);

    private:
        EuvWork m_work;
    };

    //!@brief 算法基类
    //!@details 算法基类，所有算法类建议继承该类
    class EmxAiPluginAlgBase {
    public:
        /**
         * @brief 算法构造函数
         * @param chn [in] 算法所在通道
         * @param name [in] 算法名称
         */
        EmxAiPluginAlgBase(int chn, const char *name) : m_chn(chn), m_name(name) {}

        virtual ~EmxAiPluginAlgBase() {}

        /**
         * @brief 算法创建
         * @param param [in] 插件通道参数
         */
        virtual void Create(const char *workspace, Json::Value &param) {}

        /**
         * @brief 算法销毁
         */
        virtual void Destroy() {}

        /**
         * @brief 算法参数改变
         * @param paramNew [in] 新插件通道参数
         * @return ErrCodeE
         */
        virtual ErrCodeE SetParam(const Json::Value &paramNew) { return ErrCodeE::Success; }

        const char *GetName() { return m_name.c_str(); }


    protected:
        int m_chn;
        std::string m_name;
    };

    class MediaServerAiChn : public MediaInfAi, public MediaServerModuleChnBase {
    public:
        explicit MediaServerAiChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Ai, chn) {}

        ~MediaServerAiChn() override = default;

        ErrCodeE SetParam(const Json::Value &param);

        ErrCodeE GetParam(Json::Value &param);

        ErrCodeE GetParamDefault(Json::Value &param);

        ErrCodeE GetParamRange(Json::Value &range) override;

    protected:
        ErrCodeE LoadParam(Json::Value &param);

        ErrCodeE SaveParam(const Json::Value &param);

    protected:
        std::vector<EmxAiPluginAlgBase *> m_algs;

    private:
        ErrCodeE GetVersion(std::string &version) { return ErrCodeE::Success; }
        void *GetParam() override { return nullptr; }
        void *GetNewParam() override { return nullptr; }
        int GetParamSize() override { return 0; }
        ErrCodeE OnParamChanged() override { return ErrCodeE::Success; }
    };

    class MediaServerAi : public MediaServerModuleBase {
    public:
        MediaServerAi() : MediaServerModuleBase(MediaModE::Ai, "ai"), m_handle(nullptr), m_created(false) {}
        void Create() override;
        void Destroy() override;
        ErrCodeE OnRecvReq(uint32_t method, int32_t chn, const char *data, int32_t size, std::string &out) override;

    private:
        ErrCodeE VerifyPlugin();

        void CloseOpenedSo();

    private:
        void *m_handle;
        std::vector<void *> m_depArray;
        bool m_created;
    };

}// namespace Emx

#endif//EMX_MediaServerAi_HPP
