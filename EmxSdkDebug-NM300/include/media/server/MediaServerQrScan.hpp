//
// Created by xiong on 2021/6/25.
//

#ifndef EMX_MediaServerQrScan_HPP
#define EMX_MediaServerQrScan_HPP

#include "media/common/MediaInfQrScan.hpp"
#include "MediaServerBase.hpp"
#include "core/euv/EuvWork.hpp"
#include "media/common/MediaRegion.hpp"

namespace Emx {
    class PanoQrPreProcess;

    class MediaServerQrScanChn : public MediaInfQrScan, public MediaServerModuleChnBase {
    public:
        explicit MediaServerQrScanChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::QrScan, chn) {}

        ~MediaServerQrScanChn() override = default;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }

        int GetParamSize() override { return sizeof(Param); }

        ErrCodeE OnRecvReqChn(uint32_t method, const char *data, int32_t size, std::string &out) override;

        void OnServerDestroy() override {
            Stop();
            OnDestroy();
        }

        ErrCodeE OnParamChanged() override { return ErrCodeE::Success; }

        ErrCodeE Start() override;

        ErrCodeE Stop() override;

    protected:
        /**
         * @brief 获取通道图像大小
         * @return
         */
        virtual Size GetChnImgSize() = 0;

        /**
         * @brief 获取一帧原始图像
         * @param dataY [out] 将获取到的图像的YUV中的Y通道数据放入dataY
         * @return ErrCodeE
         */
        virtual ErrCodeE GetOneImg(uint8_t **dataY) = 0;

        /**
         * @brief 释放一帧原始图像，仅当GetOneImg返回成功时此函数才会被执行
         * @param dataY [in] GetOneImg 获取到的Y数据指针
         */
        virtual void ReleaseOneImg(uint8_t *dataY) = 0;

        virtual void OnQrScanRun(void *arg);

        virtual void OnQrScanComplete(ErrCodeE e, void *arg);

        ErrCodeE Decode(uint8_t *data);

    private:
        ErrCodeE CreatePanoPreProcess(Json::Value &json);

        void DestroyPanoPreProcess();

        ErrCodeE DoDecode(void *data, int w, int h, std::string &content);

    protected:
        bool m_started;
        Size m_imgSize;
        enum class QrTypeE : uint8_t {
            Normal,
            Panorama,
        } m_type;
        PanoQrPreProcess *m_pano;
        std::string m_mapX;
        std::string m_mapY;
        EuvWork m_work;
        ErrCodeE m_getImgErrCode;
        uint8_t *m_dataY;
        bool m_got;
        std::string m_content;
        Param m_param;
        Param m_paramNew;
    };

    class MediaServerQrScan : public MediaServerModuleBase {
    public:
        MediaServerQrScan() : MediaServerModuleBase(MediaModE::QrScan, "qrScan") {}
    };
}

#endif //EMX_MediaServerQrScan_HPP
