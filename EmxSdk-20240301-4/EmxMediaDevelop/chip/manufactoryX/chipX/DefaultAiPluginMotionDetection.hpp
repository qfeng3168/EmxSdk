//
// Created by xiong on 2023/9/11.
//

#ifndef EMX_DefaultAiPluginMotionDetection_HPP
#define EMX_DefaultAiPluginMotionDetection_HPP

#include "media/server/MediaServerAi.hpp"
#include "media/common/MediaInfAiDataMotionDetection.hpp"

namespace Emx {

    class DefaultAiPluginMotionDetection : public MediaAiPlugin {
    public:
        DefaultAiPluginMotionDetection(int chn) : MediaAiPlugin(chn, "MotionDetection") {}

        virtual ~DefaultAiPluginMotionDetection() {}

        void Create(Json::Value &param) override;

        void Destroy() override;

        ErrCodeE ParamChange(Json::Value &paramNew) override;

    private:

        void Start(Json::Value &param);

        void Stop(Json::Value &param);

        void OnGetFrame(void *arg);

        void OnGetFrameComplete(void *arg);

    private:
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

}

#endif //EMX_DefaultAiPluginMotionDetection_HPP
