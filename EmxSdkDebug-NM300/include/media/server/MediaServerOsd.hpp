//
// Created by xiong on 2023/8/16.
//

#ifndef EMX_MediaServerOsd_HPP
#define EMX_MediaServerOsd_HPP

#include "media/common/MediaInfOsd.hpp"
#include "MediaServerBase.hpp"
#include "media/common/MediaRegion.hpp"
#include "core/euv/EuvTimer.hpp"
#include <list>

class OutlineOsd;

namespace Emx {

    class MediaServerOsdChn : public MediaInfOsd, public MediaServerModuleChnBase {
    public:
        explicit MediaServerOsdChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Osd, chn), m_timeRecord(0) {}

        ~MediaServerOsdChn() override = default;

        ErrCodeE LoadParam() override;

        ErrCodeE SaveParam() override;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }

        int GetParamSize() override { return sizeof(Param); }

        void NewParamInCome(const char *data, int size) override;

        bool ParamChanged() override;

        void GetParamData(std::string &data) override;

        void UpdateParam() override { m_param = m_paramNew; }

        void ReleaseNewParam() override { m_paramNew.array.clear(); }

        ErrCodeE OnServerCreate() override;

        void OnServerDestroy() override;

        ErrCodeE OnParamChanged() override;

    protected:
        /**
         * @brief 获取通道图像大小(osd所叠加的画面大小，例如vpss chn的宽高)
         * @return
         */
        virtual Size GetChnPicSize() = 0;

        /**
         * @brief 创建区域
         * @param x [in] 区域x坐标
         * @param y [in] 区域y坐标
         * @param w [in] 区域宽
         * @param h [in] 区域高
         * @param layer [in] 叠加层级
         * @param show [in] 是否显示
         * @return 区域id
         */
        virtual int RegionCreate(int x, int y, int w, int h, int layer, bool show) = 0;

        /**
         * @brief 销毁区域
         * @param region [in] RegionCreate返回的区域id
         */
        virtual void RegionDestroy(int region) = 0;

        /**
         * @brief 设置bitmap
         * @param region [in] RegionCreate返回的区域id
         * @param pix [in] 内存地址
         * @param w [in] 画面宽
         * @param h [in] 画面高
         */
        virtual void SetBitMap(int region, uint8_t *pix, int w, int h) = 0;

        /**
         * @brief 将图片更新至画布
         * @param region [in] RegionCreate返回的区域id
         * @param data [in] 图片数据
         * @return ErrCodeE
         */
        virtual ErrCodeE ImageToCanvas(int region, const std::string &data) = 0;

    private:
        void CreateTime();

        void DestroyTime();

        void CreateText();

        void DestroyText();

        void CreateImage();

        void DestroyImage();

        static void LoadParamItem(ParamItem &item, const Json::Value &json);

        void LoadParamTime(Json::Value &json);

        void LoadParamText(Json::Value &json);

        void LoadParamImage(Json::Value &json);

        static void SaveParamItem(const ParamItem &item, Json::Value &json);

        void SaveParamTime(Json::Value &json);

        void SaveParamText(Json::Value &json);

        void SaveParamImage(Json::Value &json);

        OutlineOsd *CreateOutline(int fontSize, const char *font);

        void DestroyOutline(OutlineOsd *outline);

        void GetTime(time_t time, char *formatString, char **weekFormat,  int maxSize, char *content);

        void CalcCoordinate(Margin &margin,
                            int regionWidth, int regionHeight,
                            int backgroundWidth, int backgroundHeight,
                            int &x, int &y);

    protected:
        Param *m_pParam;
        Param m_param;
        Param m_paramNew;
    private:
        Size m_chnPicSize;

        struct OutlineOsdCfg {
            OutlineOsd *outline;
            int fontSize;
            char font[32];
            int refCnt;
        };
        std::list<OutlineOsdCfg *> m_outline;

        struct OsdItem {
            OutlineOsd *outline;
            ParamItem *item;
            int handle;
        };
        std::list<OsdItem> m_osdTime;
        std::list<OsdItem> m_osdText;
        std::list<OsdItem> m_osdImage;

        EuvTimer m_timer;
        time_t m_timeRecord;
    };

    class MediaServerOsd : public MediaServerModuleBase {
    public:
        MediaServerOsd() : MediaServerModuleBase(MediaModE::Osd, "osd") {}
    };
}

#endif //EMX_MediaServerOsd_HPP