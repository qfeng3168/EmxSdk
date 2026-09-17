/*
* @Author: xiong
* @Date: 2022/9/23
*/

#ifndef EMX_TimeZone_HPP
#define EMX_TimeZone_HPP

#include "EmxCore.hpp"
#include "core/msg/MsgTopic.hpp"
#include <functional>

namespace Emx {
    constexpr uint32_t TIME_ZONE_TOPIC_ID = 0x00050001;

    /** @addtogroup comenv
     *  @{
     */

    class TimeZone {
    public:
        TimeZone() { memset(m_zone, 0, sizeof(m_zone)); }

        static const int MaxZoneSize = 128;//!<zone字符串最大大小
        /**
         * @brief 从flash中加载参数
         * @return ErrCodeE
         */
        ErrCodeE Load();

        /*!
         * @brief 设置系统时区(更改zone.json同时配置时区文件)
         * @param zone [in]时区名("GMT-8")
         * @return ErrcodeE
         */
        ErrCodeE Set(const char *zone);

        /** 应用时区到当前进程*/
        static ErrCodeE Apply(const char *zone);

        /** 发布时区变化消息，通知其他进程刷新本地时区。 */
        static ErrCodeE Publish(const char *zone);

        /*!
         * @brief 将zone.json中的时区复制到localtime文件
         * @return ErrcodeE
         */
        ErrCodeE CopyTimeZoneToLocalTime();

        /*!
         * @brief 获取当前时区文件名
         * @return 时区文件名
         */
        char *Get() { return m_zone; }

        /*!
         * @brief 获取时区文件列表
         * @param list [out] 获取时区列表
         */
        void GetZoneList(std::vector<std::string> &list);

    private:
        void GetZoneListDir(const char *path, const std::string &name, std::vector<std::string> &list);

        char m_zone[MaxZoneSize];
    };

    class TimeZoneMonitor {
    public:
        using OnChanged = std::function<void(const char *zone)>;

        ErrCodeE Create(EuvLoop &loop, char *buffer, int bufferSize, OnChanged cb = nullptr);

        void Destroy();

    private:
        MsgTopicSubscriberAsync m_sub;
        OnChanged m_cb;
    };

    //!@brief 根据用户输入的GMT时区信息和夏令时信息自动生成时区文件并配置localtime
    class TimeZone2 {
        // tz-data
        // # Rule  NAME    FROM    TO      TYPE    IN      ON      AT      SAVE    LETTER/S
        // Rule	   EMX     1970    max     -       Jan     25      18:14   1:00    D
        // Rule	   EMX     1970    max     -       Jan     25      19:15   0       S

        // # Zone  NAME        STDOFF   RULES   FORMAT  [UNTIL]
        // Zone    localtime   8:00	    EMX	    C%sT
        //
    public:
        struct ZoneParam {
            char gmt[12];// eg."GMT+08:45"
            struct Daylight {
                bool ena;
                char save[8];// 夏令时调整时常eg."1:10"，调快一个小时零十分钟
                struct Rule {
                    char in[4]; // 规则月份 eg."Jan"/"Feb"/"Aug"...
                    char on[12];// 规则日期，这部分规则比较复杂
                    char at[8]; // 规则时间 eg."2:00"，凌晨2点
                    // on 规则日期解析
                    // 当on为具体数字时，代表这个月的几号，例如"25"代表in这个月的25开始/结束
                    // 夏令时一般不是具体几号，而是以某个周日或周几为开始/结束
                    // on支持这种方式，例如"Sun>=1"代表in这个月1号或1号后的首个周日开始结束
                    // "Thu>19"代表in这个月19号后的首个周四开始/结束
                    // 另外还可以使用lastXxx规则
                    // "lastXxx"代表这个月的最后一个周几，例如"lastSun"代表最后一个周日
                } into, leave;//进入离开夏令时规则
            } daylight;
        };
        TimeZone2() {}
        virtual ~TimeZone2() {}

        static ErrCodeE InitDeviceTimeZone();

        /*!
         * @brief 设置系统时区
         * @param param [in] 参数
         * @return ErrcodeE
         */
        static ErrCodeE Set(const ZoneParam &param);

        /*!
         * @brief 获取当前时区参数
         * @return 参数
         */
        static ErrCodeE Get(ZoneParam &param) { return Load(param); }

    private:
        static ErrCodeE Load(ZoneParam &param);
        static ErrCodeE GenerateLocaltime(const ZoneParam &param);
    };
    /** @}*/
}// namespace Emx

#endif//EMX_TimeZone_HPP
