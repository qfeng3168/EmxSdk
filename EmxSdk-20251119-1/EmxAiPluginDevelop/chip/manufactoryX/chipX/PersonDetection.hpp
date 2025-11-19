//
// Created by xiong on 2023/9/11.
//

#ifndef EMX_PersonDetection_HPP
#define EMX_PersonDetection_HPP

#include "media/server/MediaServerAi.hpp"
#include "media/common/MediaRegion.hpp"
#include "media/common/MediaInfAiDataPersonDetection.hpp"

namespace Emx {

    class PersonDetection : public EmxAiPluginAlgBase {
    public:
        PersonDetection(int chn) : EmxAiPluginAlgBase(chn, "PersonDetection"), m_created(false){}

        virtual ~PersonDetection() {}

        void Create(const char *workspace, Json::Value &param) override;

        void Destroy() override;

        ErrCodeE SetParam(const Json::Value &paramNew) override;

    private:
        void Start(const Json::Value &param);

        void Stop(const Json::Value &param);

        void OnGetFrame(void *arg);

        void OnGetFrameComplete(void *arg);

    private: 
        bool m_created;
        int m_vpssGrp;
        int m_vpssChn;
        int m_modelId;
        std::string m_modePath;
        float m_threshold;
        bool m_got;
        bool m_ena;
        Json::Value m_param;
        MediaAiJob m_job;
        MediaInfAiDataPersonDetection m_data;
    };

}// namespace Emx

#endif//EMX_PersonDetection_HPP
