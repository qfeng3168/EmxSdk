//
// Created by xiong on 2023/9/11.
//

#ifndef EMX_PersonDetection_HPP
#define EMX_PersonDetection_HPP

#include "media/common/MediaInfAiDataPersonDetection.hpp"
#include "media/server/MediaServerAi.hpp"
namespace Emx {

    class PersonDetection : public EmxAiPluginAlgBase {
    public:
        PersonDetection(int chn) : EmxAiPluginAlgBase(chn, "PersonDetection"), m_created(false), m_started(false) {}

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
        bool m_started;
        int m_vpssGrp;
        int m_vpssChn;
        int m_interval;
        int m_threshold;
        int m_minArea;
        bool m_got;
        Json::Value m_param;
        MediaAiJob m_job;
        MediaInfAiDataPersonDetection m_data;
    };

}// namespace Emx

#endif//EMX_PersonDetection_HPP
