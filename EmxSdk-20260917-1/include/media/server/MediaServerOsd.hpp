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
#include <thread>
#include <vector>
#include <mutex>

class OutlineOsd;

namespace Emx {
    /** @addtogroup media-server
     *  @{
     */

    class MediaServerOsdChn : public MediaInfOsd, public MediaServerModuleChnBase {
    public:
        explicit MediaServerOsdChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Osd, chn), m_timeRecord(0) {}

        ~MediaServerOsdChn() override = default;

        ErrCodeE LoadParam() override;

        ErrCodeE SaveParam() override;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }

        void *GetRunningParam() override { return &m_paramRunning; }

        ErrCodeE SetOsdContent(OsdContent *pInfo) override { return ErrCodeE::Success; }

        ErrCodeE OnRecvReqChn(uint32_t method, const char *data, int32_t size, std::string &out) override;

        int GetParamSize() override { return -1; }

        ErrCodeE GetParamDefaultData(std::string &data) override;

        void NewParamInCome(const char *data, int size) override;

        bool ParamChanged() override;

        void GetParamData(std::string &data) override;

        void GetRunningParamData(std::string &data) override;

        void UpdateParam() override;

        void UpdateRunningParam() override {};

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
            MediaInfOsd::TypeE type; //!< OSD类型
            int x;                   //!< 区域x坐标
            int y;                   //!< 区域y坐标
            int w;                   //!< 区域宽
            int h;                   //!< 区域高
            int layer;               //!< 叠加层级
            bool show;               //!< 是否显示
            int id;                  //!< region的唯一标识
            uint16_t flickerInterval;//!<闪烁间隔，单位毫秒
            uint8_t isFlicker;       //!<是否闪烁显示
            uint8_t autoSwitchColor; //!< 是否自动黑白颜色转换
        };
        struct SwitchColorRect {
            uint16_t x;//!< 区域起始x坐标
            uint16_t y;//!< 区域起始y坐标
            uint16_t w;//!< 区域宽
            uint16_t h;//!< 区域高
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
         * @param switchColorRectList [in] 自动黑白颜色转换的坐标宽高度列表,每一个宽代表一个完整的字符或汉字
         */
        virtual void SetBitMap(int id, uint8_t *pix, int w, int h,
                std::vector<SwitchColorRect> *switchColorRectList = nullptr) = 0;

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
        /**
         * @brief 创建所有相关资源
         * 该方法用于初始化并创建所有需要的资源，如区域、字体等。
         */
        void CreateAll();

        /**
         * @brief 销毁所有相关资源
         * 该方法用于释放之前创建的所有资源，避免资源泄漏。
         */
        void DestroyAll();

        /**
         * @brief 对所有资源进行预处理
         * 该方法用于对所有资源进行预处理操作，如调整参数、计算坐标等。
         */
        void PreproccessAll();

        /**
         * @brief 释放所有资源
         * 该方法用于释放所有占用的资源，确保资源被正确回收。
         */
        void ResourceFreeAll();

        /**
         * @brief 线程写入操作
         * 该方法用于在单独的线程中进行写入操作，以提高性能。
         */
        void ThreadWrite();

        OutlineOsd *CreateOutline(int fontSize, uint32_t color, const char *font);

        /**
         * @brief 销毁Outline对象
         * @param outline 要销毁的Outline对象指针
         * 该方法用于释放指定的Outline对象占用的资源。
         */
        void DestroyOutline(OutlineOsd *outline);

        /**
         * @brief 获取时间信息
         * @param time 时间戳
         * @param formatString 时间格式字符串
         * @param weekFormat 星期格式字符串数组
         * @param maxSize 内容最大长度
         * @param content 存储时间信息的字符串
         * 该方法根据指定的时间戳、格式字符串和星期格式，将时间信息填充到content中。
         */
        void GetTime(time_t time, char *formatString, char weekFormat[7][16], int maxSize, char *content,
                bool useMaxWeek = false);

        /**
         * @brief 计算坐标
         * @param margin 边距
         * @param regionWidth 区域宽度
         * @param regionHeight 区域高度
         * @param backgroundWidth 背景宽度
         * @param backgroundHeight 背景高度
         * @param x 输出的x坐标
         * @param y 输出的y坐标
         * 该方法根据给定的边距、区域大小和背景大小，计算出合适的x和y坐标。
         */
        void CalcCoordinate(Margin &margin, int regionWidth, int regionHeight, int backgroundWidth,
                int backgroundHeight, int &x, int &y);

        /**
         * @brief 设置运行时参数
         * @param param 要设置的参数指针
         * 该方法用于设置运行时的参数，更新系统的运行状态。
         */
        void SetRunningParam(Param *param);

        /**
         * @brief 将参数压入队列
         * 该方法将参数压入队列，以便后续处理。
         */
        void ParamPush();

        /**
         * @brief 从队列中弹出参数
         * @return 如果成功弹出参数返回true，否则返回false
         * 该方法从队列中弹出一个参数，如果队列不为空则返回true，否则返回false。
         */
        bool ParamPop();
        /**
         * @brief 启动线程执行写入操作
         * 该方法用于启动一个线程，在线程中调用 ThreadWrite 方法进行写入操作。
         */
        void ThreadRun();
        /**
         * @brief 停止线程执行写入操作
         * 该方法用于停止线程的执行，确保线程正常退出。
         */
        void ThreadStop();
        /**
         * @brief 时间更新
         * 该方法用于更新时间相关的信息。
         */
        void TimeUpdata();
        /**
         * @brief 文本更新
         * 该方法用于更新文本相关的信息。
         */
        void TextUpdata();

        /**
         * @brief 内容压入队列
         * @param info 要压入的内容信息
         * 该方法将内容信息压入队列，以便后续处理。
         */
        void ContentPush(OsdContent &info);

    protected:
        Param *m_pParam;
        Param m_param;
        Param m_paramNew;
        Param m_paramRunning;
        Param m_paramTread;

    private:
        Size m_chnPicSize;

        struct OutlineOsdCfg {
            OutlineOsd *outline;
            int fontSize;
            char font[32];
            int refCnt;
            uint32_t color;//!< 字体轮廓配置中的颜色，使用 32 位无符号整数表示
        };
        std::list<OutlineOsdCfg *> m_outline;//!< 存储 OutlineOsdCfg 指针的列表，用于管理所有的字体轮廓配置

        /**
         * @brief 表示一个 OSD 项的结构体
         * 包含了 OSD 项的字体轮廓、参数项、区域信息和在参数数组中的下标
         */
        struct OsdItem {
            OutlineOsd *outline;//!< 指向 OutlineOsd 对象的指针，表示该项使用的字体轮廓
            ParamItem *item;    //!< 指向 ParamItem 对象的指针，表示该项的参数
            RegionInfo *region; //!< 指向 RegionInfo 对象的指针，表示该项所在的区域
            std::vector<SwitchColorRect> switchColorRect;//!< 存储切换颜色的矩形列表
            int index;          //!< 该项在参数数组中的下标
        };

        std::list<OsdItem> m_osdTime;  //!< 存储时间相关 OSD 项的列表
        std::list<OsdItem> m_osdText;  //!< 存储文本相关 OSD 项的列表
        std::list<OsdItem> m_osdImage; //!< 存储图像相关 OSD 项的列表
        std::list<RegionInfo> m_region;//!< 存储所有区域信息的列表

        EuvTimer m_timer;                      //!< 定时器对象
        time_t m_timeRecord;                   //!< 时间记录
        bool m_threadRunning = false;          //!< 线程运行标志
        bool m_craeteFlag = false;             //!< 创建标志
        std::thread m_thread;                  //!< 线程对象
        std::vector<Param> m_paramQueue;       //!< 数据队列
        std::mutex m_paramMtx;                 //!< 数据同步锁
        std::vector<OsdContent> m_contentQueue;//!< 内容队列
        std::string m_cacheChar;               //!< 缓存字符串
        int m_thickness;                       //!< 字体轮廓宽度

    private:
        /**
         * @brief 带换行字符串切割，返回实际map size
         * 根据指定字符串来分割原始字符串，得到
         * @param paramText [in]被分割字符串对象
         * @param region [out]返回需要使用的宽高
         * @param outline [in]字体创建句柄
         * @return 返回字数
         */
        int TextWrapLineSize(ParamText &paramText, RegionInfo &region, OutlineOsd *outline);
        /**
         * @brief 带换行字符串切割，返回bitmap
         * 根据指定字符串来分割原始字符串，并直接创建位图
         * @param text [in]被分割字符串对象
         * @param w [out]重组之后的bitmap宽
         * @param h [out]重组之后的bitmap高
         * @return -1 失败 0 成功
         */
        int TextWrapLine(OsdItem &text);
    };

    class MediaServerOsd : public MediaServerModuleBase {
    public:
        MediaServerOsd() : MediaServerModuleBase(MediaModE::Osd, "osd") {}
    };
    /** @}*/
}// namespace Emx

#endif//EMX_MediaServerOsd_HPP