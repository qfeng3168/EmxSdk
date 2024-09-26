//
// Created by tangfahong on 2024/4/22.
//

#ifndef EMX_FaceAlgorithm_HPP
#define EMX_FaceAlgorithm_HPP

#include "media/common/MediaInfAiDataFaceDetection.hpp"
#include "media/common/MediaInfAiDataFaceRecognition.hpp"
#include "media/server/MediaServerAi.hpp"

namespace Emx {

    class FaceAlgorithm : public EmxAiPluginAlgBase {
    public:
        FaceAlgorithm(int chn) : EmxAiPluginAlgBase(chn, "FaceAlgorithm"), m_created(false), m_started(false) {}

        virtual ~FaceAlgorithm() {}

        void Create(const char *workspace, Json::Value &param) override;

        void Destroy() override;

        ErrCodeE SetParam(const Json::Value &paramNew) override;

        ErrCodeE Ctrl(const Json::Value &in, Json::Value &out) override;

    public:
        struct FaceRecognition {
            bool ena;           //!<人脸抓拍开关
            bool captureBackImg;//!<人脸抓拍背景大图
            bool captureRectImg;//!<人脸抓拍抠图
            bool aeEna;         //!<人脸抓拍曝光控制
            bool maskEna;       //!<口罩检测开关
            bool recognitionEna;//!<人脸识别比对开关

            uint8_t maskThreshold;//!<口罩检测阈值
            uint8_t aeThreshold;  //!<人脸抓拍曝光等级[0~255]
            uint8_t threshold;    //!<人脸抓拍阈值[0~100]
            uint8_t snapMode;     //!<人脸抓拍模式 0=快速,1=质量,2=定时

            uint16_t snapTime;  //!<人脸抓拍间隔时间，一般是帧数
            uint8_t snapQuality;//!<人脸抓拍质量等级[0~100]
            uint8_t snapClarity;//!<人脸抓拍清晰度阈值[0~100]

            uint8_t snapAngle;     //!<人脸抓拍角度限制[0~255]
            uint8_t captureQuality;//!<人脸抓拍图片质量[0~100]
            uint16_t minWidth;     //!<人脸抓拍最小像素宽
            uint16_t minHeight;    //!<人脸抓拍最小像素高

        };
        struct FaceDetection {
            bool ena;          //!<人脸检测开关
            uint16_t minWidth; //!<人脸检测最小像素宽
            uint16_t minHeight;//!<人脸检测最小像素高
        };

    private:
        void Start(const Json::Value &param);

        void Stop();

        void OnGetFrame(void *arg);

        void OnGetFrameComplete(void *arg);

        ErrCodeE FaceRegister(const Json::Value &info, Json::Value &out);

        ErrCodeE FaceUnregister(const Json::Value &id);

        ErrCodeE FaceSyncLibrary(const Json::Value &library);

        ErrCodeE SetFaceParam(const Json::Value &param);

        ErrCodeE SetFaceDetectionParam(const Json::Value &param);

        int FaceAeCtrl(void *faces, void *pstFrame);

        int FaceModelInit();
        /**
         * @brief 人脸处理
         * 通过人脸的质量 正脸度 和尺寸大小判断，初次撒选人脸
         * @param pstfdFrame 原始视频数据
         * @return int 0=成功 其他=失败
         */
        int FaceManage(void *pstFrame);

        /**
         * @brief 人脸检测处理
         */
        void FaceDetectionManage();
    private:
        bool m_created;
        bool m_started;
        float m_thresholdFace;
        int m_aiHandle;         //!<ai句柄
        int m_appHandle;    //!< app算法句柄，相当于又封装了一层m_aiHandle
        int m_chn = 0;                     //!< 算法通道
        int m_vbId;                        //!< 算法指定使用vbId，-1=不指定
        int m_vpssGrp;                     //!< 算法获取图像源组
        int m_vpssChn;                     //!< 算法获取图像源通道
        std::string m_snapModelPath;       //!< 抓拍算法模型
        std::string m_recognitionModelPath;//!< 识别算法模型
        std::string m_maskModelPath;       //!< 口罩算法模型
        MediaAiJob m_job;

    private:
        FaceRecognition m_faceRecognition;              //!<人脸识别param
        bool m_gotFaceRecognition;                      //!< 人脸识别推送标志位
        MediaInfAiDataFaceRecognition m_dataRecognition;//!<人脸识别结果缓存

    private:
        FaceDetection m_faceDetection;                  //!<人脸检测param
        bool m_gotFaceDetection;                        //!< 人脸检测推送标志位
        MediaInfAiDataFaceDetection m_dataFaceDetection;//!< 人脸检测结果缓存
    };

}// namespace Emx

#endif//EMX_FaceAlgorithm_HPP
