//
// Created by xiong on 2023/9/11.
//

#ifndef EMX_MotionDetection_HPP
#define EMX_MotionDetection_HPP

#include "media/common/MediaInfAiDataMotionDetection.hpp"
#include "media/server/MediaServerAi.hpp"
namespace Emx {

    class MotionDetection : public EmxAiPluginAlgBase {
    public:
        MotionDetection(int chn) : EmxAiPluginAlgBase(chn, "MotionDetection"), m_created(false), m_started(false) {}

        virtual ~MotionDetection() {}

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
        MediaInfAiDataMotionDetection m_data;
    };

}// namespace Emx

#endif//EMX_MotionDetection_HPP
