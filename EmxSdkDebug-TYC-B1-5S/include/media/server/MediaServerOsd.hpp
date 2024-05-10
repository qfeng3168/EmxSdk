//
// Created by xiong on 2023/8/16.
//

#ifndef EMX_MediaServerOsd_HPP
#define EMX_MediaServerOsd_HPP

#include "MediaServerBase.hpp"
#include "core/euv/EuvTimer.hpp"
#include "media/common/MediaInfOsd.hpp"
#include "media/common/MediaRegion.hpp"
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

        int GetParamSize() override { return -1; }

        ErrCodeE GetParamDefaultData(std::string &data) override;

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

        struct RegionInfo {
            MediaInfOsd::TypeE type;//!< OSD类型
            int x;                  //!< 区域x坐标
            int y;                  //!< 区域y坐标
            int w;                  //!< 区域宽
            int h;                  //!< 区域高
            int layer;              //!< 叠加层级
            bool show;              //!< 是否显示
            int id;                 //!< region的唯一标识
        };

        /**
         * @brief 创建所有region，并填充id字段，用于标识不同的region
         * @param region [in] region列表
         */
        virtual void RegionCreateAll(std::list<RegionInfo> &region) {}

        /**
         * @brief 销毁所有region
         */
        virtual void RegionDestroyAll(const std::list<RegionInfo> &region) {};

        /**
         * @brief 设置bitmap
         * @param id [in] RegionCreate返回的区域id
         * @param pix [in] 内存地址
         * @param w [in] 画面宽
         * @param h [in] 画面高
         */
        virtual void SetBitMap(int id, uint8_t *pix, int w, int h) = 0;

        /**
         * @brief 将图片更新至画布
         * @param id [in] RegionCreate返回的区域id
         * @param data [in] 图片数据
         * @return ErrCodeE
         */
        virtual ErrCodeE ImageToCanvas(int id, const std::string &data) = 0;

        /**
         * @brief 暂停OSD通道，用于编码通道切换分辨率时停止osd，可视情况选择是否实现
         */
        virtual void Pause() {}

        /**
         * @brief 恢复OSD通道，用于编码通道切换分辨率后恢复osd，可视情况选择是否实现
         */
        virtual void Resume() {}

    private:
        void CreateAll();

        void DestroyAll();

        OutlineOsd *CreateOutline(int fontSize, const char *font);

        void DestroyOutline(OutlineOsd *outline);

        void GetTime(time_t time, char *formatString, char weekFormat[7][16], int maxSize, char *content);

        void CalcCoordinate(Margin &margin, int regionWidth, int regionHeight, int backgroundWidth,
                int backgroundHeight, int &x, int &y);

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
            RegionInfo *region;
        };

        std::list<OsdItem> m_osdTime;
        std::list<OsdItem> m_osdText;
        std::list<OsdItem> m_osdImage;
        std::list<RegionInfo> m_region;

        EuvTimer m_timer;
        time_t m_timeRecord;

    private:
        /**
         * @brief 带换行字符串切割，返回实际map size
         * 根据指定字符串来分割原始字符串，得到
         * @param paramText [in]被分割字符串对象
         * @param region [out]返回需要使用的宽高
         * @param outline [in]字体创建句柄
         */
        void TextWrapLineSize(ParamText &paramText, RegionInfo &region, OutlineOsd *outline);
         /**
         * @brief 带换行字符串切割，返回bitmap
         * 根据指定字符串来分割原始字符串，并直接创建位图
         * @param paramText [in]被分割字符串对象
         * @return 返回位图数据
         */
        uint8_t *TextWrapLine(OsdItem &text, int *w, int *h);
    };

    class MediaServerOsd : public MediaServerModuleBase {
    public:
        MediaServerOsd() : MediaServerModuleBase(MediaModE::Osd, "osd") {}
    };
}// namespace Emx

#endif//EMX_MediaServerOsd_HPP