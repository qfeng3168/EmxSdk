//
// Created by xiong on 2023/9/13.
//

#ifndef EMX_DefaultAiPluginPersonDetection_HPP
#define EMX_DefaultAiPluginPersonDetection_HPP

#include "media/server/MediaServerAi.hpp"
#include "media/common/MediaRegion.hpp"
#include "media/common/MediaInfAiDataPersonDetection.hpp"

namespace Emx {

    class DefaultAiPluginPersonDetection : public MediaAiPlugin {
    public:
        DefaultAiPluginPersonDetection(int chn) : MediaAiPlugin(chn, "PersonDetection") {}

        virtual ~DefaultAiPluginPersonDetection() {}

        void Create(Json::Value &param) override;

        void Destroy() override;

        ErrCodeE ParamChange(Json::Value &paramNew) override;

    private:

        void Start(Json::Value &param);

        void Stop(Json::Value &param);

        void OnGetFrame(void *arg);

        void OnGetFrameComplete(void *arg);

    private:
        int m_chn;
        int m_vpssGrp;
        int m_vpssChn;
        int m_threshold;
        bool m_got;
        Json::Value m_param;
        MediaAiJob m_job;
        MediaInfAiDataPersonDetection m_data;
    };

}

#endif //EMX_DefaultAiPluginPersonDetection_HPP
