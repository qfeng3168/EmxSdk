//
// Created by xiong on 2022/6/9.
//

#ifndef EMX_Reboot_HPP
#define EMX_Reboot_HPP

#include "core/msg/MsgTopic.hpp"
#include <functional>


namespace Emx {
    /** @addtogroup utils
     *  @{
     */
    class Reboot {
    public:
        /*!
         * @brief 重启系统
         * @param delayS [in] 几s后重启，默认立即重启
         * @param async [in] 是否异步进行重启
         */
        static void DoReboot(int delayS = 0, bool async = true);

    private:
        static void Rebooting(int delayS);
    };

    class RebootMonitor {
    public:
        RebootMonitor() {}

        virtual ~RebootMonitor() {}

        using OnReboot = std::function<void()>;

        void Create(EuvLoop &loop, char *buffer, int bufferSize, OnReboot cb);

        void Destroy();

    private:
        MsgTopicSubscriberAsync m_sub;
        OnReboot m_cb;
    };

    /** @}*/
}// namespace Emx

#endif//EMX_Reboot_HPP
