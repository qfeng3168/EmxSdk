//
// Created by tangfahong on 2024/4/1.
//

#ifndef EMX_PassengerFlowStatistics_HPP
#define EMX_PassengerFlowStatistics_HPP

#include "media/server/MediaServerAi.hpp"
#include "media/common/MediaRegion.hpp"
#include "media/common/MediaInfAiDataPassengerFlowStatistics.hpp"

namespace Emx {

    class PassengerFlowStatistics : public EmxAiPluginAlgBase {
    public:
        PassengerFlowStatistics(int chn) : EmxAiPluginAlgBase(chn, "PassengerFlowStatistics"), m_created(false) {}

        virtual ~PassengerFlowStatistics() {}

        void Create(const char *workspace, Json::Value &param) override;

        void Destroy() override;

        ErrCodeE SetParam(const Json::Value &paramNew) override;

        ErrCodeE Ctrl(const Json::Value &in, Json::Value &out) override;
    private:

        void Start(const Json::Value &param);

        void Stop(const Json::Value &param);

        void OnGetFrame(void *arg);

        void OnGetFrameComplete(void *arg);

        ErrCodeE ParamSet();
    private:
        bool m_created;
        int m_aiHandle;  //!<ai句柄
        int m_appHandle; //!< app算法句柄，相当于又封装了一层m_aiHandle
        int m_vbId; //!< 算法指定使用vbId，-1=不指定
        int m_vpssGrp;  //!< 算法获取图像源组
        int m_vpssChn;  //!< 算法获取图像源通道
        std::string m_modePath;
        float m_threshold;  //!< 灵敏度阈值
        uint32_t m_width;   //!< 算法使用分辨率宽
        uint32_t m_height;  //!< ...高
        bool m_got;         //!<处理状态
        bool m_ena;         //!<使能状态
        Json::Value m_param;    //!<缓存param
        MediaAiJob m_job;       
        MediaInfAiDataPassengerFlowStatistics m_data;   //!<数据结果缓存

        uint32_t m_entryNum = 0; //!<客流统计进入人数
        uint32_t m_leaveNum = 0;  //!<客流统计离开人数
    };

}

#endif //EMX_PassengerFlowStatistics_HPP
