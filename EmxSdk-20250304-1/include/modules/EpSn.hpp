//
// Created by xiong on 2022/6/10.
//

#ifndef EMX_EpSn_HPP
#define EMX_EpSn_HPP

#include "EmxCore.hpp"

namespace Emx {

    //!@brief 提供设备EpSn的获取与配置
    class EpSn {
    public:
        EpSn() {
            memset(m_epSn, 0, sizeof(m_epSn));
        }

        static const int MaxEpSnSize = 128;//!<EpSn字符串最大大小

        /**
         * @brief 从flash中加载参数
         * @return ErrCodeE
         */
        ErrCodeE Load();

        /**
         * @brief 获取EpSn
         * @return EpSn
         */
        char *Get() { return m_epSn; }

        /**
         * @brief 设置EpSn
         * @param sn [in] EpSn
         * @return ErrCodeE
         */
        ErrCodeE Set(const char *epSn);

    private:
        char m_epSn[MaxEpSnSize];
    };

}

#endif //EMX_EpSn_HPP
