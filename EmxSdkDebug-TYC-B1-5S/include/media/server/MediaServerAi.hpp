//
// Created by xiong on 2020/10/27.
//

#ifndef EMX_MediaServerAi_HPP
#define EMX_MediaServerAi_HPP

#include "MediaServerBase.hpp"
#include "core/euv/EuvWork.hpp"
#include "media/common/MediaInfAiData.hpp"
#include "media/common/MediaRingBuffer.hpp"

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

    class MediaAiPlugin {
    public:
        MediaAiPlugin(int chn, const char *name) : m_chn(chn), m_name(name) {}

        virtual ~MediaAiPlugin() {}

        /**
         * @brief 插件创建
         * @param param [in] 插件的参数
         */
        virtual void Create(Json::Value &param) {}

        /**
         * @brief 插件销毁
         */
        virtual void Destroy() {}

        /**
         * @brief 插件参数改变
         * @param paramNew [in] 新参数
         * @return ErrCodeE
         */
        virtual ErrCodeE ParamChange(Json::Value &paramNew) { return ErrCodeE::Success; }

        const char *GetName() { return m_name.c_str(); }

    protected:
        int m_chn;
        std::string m_name;
    };

    class MediaServerAiChn : public MediaServerModuleChnBase {
    public:
        explicit MediaServerAiChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Ai, chn) {}

        ~MediaServerAiChn() override = default;

        /**
         * @brief 创建一个Ai插件
         * @param name [in] Ai插件名
         * @return ErrCodeE
         */
        virtual MediaAiPlugin *MakePlugin(const char *name) = 0;

        ErrCodeE OnCreate() override;

        void OnDestroy() override;

        ErrCodeE OnParamChanged() override;

        ErrCodeE LoadParam() override;

        ErrCodeE SaveParam() override;

        ErrCodeE GetParamRange(Json::Value &range) override { return ErrCodeE::Success; }

        void *GetParam() override { return nullptr; }

        void *GetNewParam() override { return nullptr; }

        int GetParamSize() override { return 0; }

        ErrCodeE GetParamDefaultData(std::string &data) override;

        void NewParamInCome(const char *data, int size) override { EasyJson::Parse(data, size, m_paramNew); }

        void GetParamData(std::string &data) override { EasyJson::Serialize(m_param, data); }

        bool ParamChanged() override { return true; }

        void UpdateParam() override {}

        void ReleaseNewParam() override { m_paramNew.clear(); }

    protected:
        Json::Value m_param;
        Json::Value m_paramNew;
        std::list<MediaAiPlugin *> m_pluginList;
    };

    class MediaServerAi : public MediaServerModuleBase {
    public:
        MediaServerAi() : MediaServerModuleBase(MediaModE::Ai, "ai") {}
    };

}// namespace Emx

#endif//EMX_MediaServerAi_HPP
