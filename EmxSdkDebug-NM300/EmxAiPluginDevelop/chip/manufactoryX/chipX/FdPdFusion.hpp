//
// Created by xiong on 2023/9/11.
//

#ifndef EMX_FdPdFusion_HPP
#define EMX_FdPdFusion_HPP

#include "media/common/MediaInfAiDataFaceSnap.hpp"
#include "media/common/MediaInfAiDataPersonDetection.hpp"
#include "media/server/MediaServerAi.hpp"
namespace Emx {
    //! 这个示例中一个算法FdPdFusion会输出两种数据，PersonDetection数据和FaceSnap数据，
    class FdPdFusion : public EmxAiPluginAlgBase {
    public:
        FdPdFusion(int chn) : EmxAiPluginAlgBase(chn, "FdPdFusion"), m_created(false), m_started(false) {}

        virtual ~FdPdFusion() {}

        void Create(const char *workspace, Json::Value &param) override;

        void Destroy() override;

        ErrCodeE SetParam(const Json::Value &paramNew) override;
        
        ErrCodeE Ctrl(const Json::Value &in, Json::Value &out) override;
    private:
        void Start(const Json::Value &param);

        void Stop(const Json::Value &param);

        void OnGetFrame(void *arg);

        void OnGetFrameComplete(void *arg);
        
        ErrCodeE FaceRegister(const Json::Value &info, Json::Value &out);

        ErrCodeE FaceUnregister(const Json::Value &id);

        ErrCodeE FaceSyncLibrary(const Json::Value &library);
    private:
        bool m_created;
        bool m_started;
        int m_vpssGrp;
        int m_vpssChn;
        int m_interval;
        int m_threshold;
        int m_minArea;
        bool m_ena;
        Json::Value m_param;
        MediaAiJob m_job;
        bool m_gotPersonDetection;
        MediaInfAiDataPersonDetection m_dataPersonDetection;
        bool m_gotFaceSnap;
        MediaInfAiDataFaceSnap m_dataFaceSnap;
    };

}// namespace Emx

#endif//EMX_FdPdFusion_HPP
