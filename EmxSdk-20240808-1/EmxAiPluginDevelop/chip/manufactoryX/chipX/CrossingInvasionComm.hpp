//
// Created by tangfahong on 2024/4/1.
//

#ifndef EMX_DefaultAiHandle_HPP
#define EMX_DefaultAiHandle_HPP
#include "media/common/MediaInfAiDataCrossingDetection.hpp"
#include "media/common/MediaInfAiDataInvasionDetection.hpp"
#include "media/common/MediaRegion.hpp"
#include "media/server/MediaServerAi.hpp"

namespace Emx {

    class CrossingInvasionComm : public EmxAiPluginAlgBase {
    public:
        CrossingInvasionComm(int chn) : EmxAiPluginAlgBase(chn, "CrossingInvasionComm") {}

        ~CrossingInvasionComm() {}

        void Create(const char *workspace, Json::Value &param) override;

        void Destroy() override;

        ErrCodeE SetParam(const Json::Value &paramNew) override;
        typedef struct {
            float x1;
            float y1;
            float x2;
            float y2;
        } ai_bbox_t;
    private:
        int Start();

        void Stop();

        void OnGetFrame(void *arg);

        void OnGetFrameComplete(void *arg);

        ErrCodeE CrossingParamSet(const Json::Value &params);

        ErrCodeE InvasionParamSet(const Json::Value &params);

        bool TypeFiltration(int type, int classes);

        bool AreaCheck(ai_bbox_t &rect, uint32_t srcWidth, uint32_t srcHeight);

        bool InvadeMap(ai_bbox_t &rect, uint32_t width, uint32_t height);

        void InvasionFrameComplete(void *aiObject, void *pstFrame);

        void CrossingFrameComplete(void *aiObject, void *pstFrame);

    private:
        bool m_created = false;
        bool m_start = false;
        int m_aiHandle;     //!<ai句柄
        int m_appHandle;//!< app算法句柄，相当于又封装了一层m_aiHandle
        int m_chn = 0;                 //!< 算法通道
        int m_vbId;                    //!< 算法指定使用vbId，-1=不指定
        int m_vpssGrp;                 //!< 算法获取图像源组
        int m_vpssChn;                 //!< 算法获取图像源通道
        std::string m_modePath;
        float m_threshold;
        uint32_t m_width; //!< 算法使用分辨率宽
        uint32_t m_height;//!< ...高
        MediaAiJob m_job;

        bool m_crossingGot;
        bool m_invasionGot;

        MediaInfAiDataCrossingDetection m_crossingData;//!<越界数据结果缓存
        MediaInfAiDataInvasionDetection m_invasionData;//!<入侵数据结果缓存

    private:
        static const int mapSizeMax = 1200;
        static const int mapWidth = 40;
        static const int mapHeight = 30;

        bool m_invasionEna = false;
        int m_invasionType = 0;//检测类型1=人 2=机动车 4=非机动车
        uint8_t m_invasionMap[mapSizeMax];

        bool m_crossEna = false;
        int m_crossType;//检测类型1=人 2=机动车 4=非机动车
    };

}// namespace Emx

#endif//EMX_DefaultAiHandle_HPP
