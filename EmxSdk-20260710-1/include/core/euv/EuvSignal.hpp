//
// Created by xiong on 2024/1/17.
//

#ifndef EMX_EUVSIGNAL_HPP
#define EMX_EUVSIGNAL_HPP

#include "EuvLoop.hpp"

namespace Emx {
    /** @addtogroup euv
     *  @{
     */

    //!@brief 实现基于Loop的异步事件通知
    class EuvSignal {
    public:
        //!@brief 事件到达时触发的回调函数
        using Callback = std::function<void(int signum)>;

        EuvSignal() : m_signal(nullptr) {}

        virtual ~EuvSignal() { Destroy(); }

        /*!
         * @brief 创建信号监听
         * @param loop [in] 需要绑定的loop
         * @param cb [in] 注册一个回调函数，当信号触发时执行
         * @return ErrCodeE
         */
        ErrCodeE Create(EuvLoop &loop, Callback cb);

        //!@brief 销毁异步通知
        void Destroy();
        /*!
         * @brief 开始监听
         * @param signum [in] 监听的信号值
         */
        void Start(int signum);

        /*!
         * @brief 停止监听
         */
        void Stop() {
            if (m_signal) uv_signal_stop(m_signal);
        }

    private:
        static void OnDestroy(uv_handle_t *handle);

    private:
        uv_signal_t *m_signal;
        Callback m_cb;
    };
    /** @}*/
}// namespace Emx
#endif//EMX_EUVSIGNAL_HPP
