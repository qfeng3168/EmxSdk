//
// Created by xiong on 2022/6/10.
//

#ifndef EMX_OemId_HPP
#define EMX_OemId_HPP

#include "EmxCore.hpp"

namespace Emx {

    //!@brief 提供设备OemId的获取与配置
    class OemId {
    public:
        OemId() {
            memset(m_oemId, 0, sizeof(m_oemId));
        }

        static const int MaxOemIdSize = 128;//!<OemId字符串最大大小

        /**
         * @brief 从flash中加载参数
         * @return ErrCodeE
         */
        ErrCodeE Load();

        /**
         * @brief 获取OemId
         * @return OemId
         */
        char *Get() { return m_oemId; }

        /**
         * @brief 设置OemId
         * @param oemId [in] OemId
         * @return ErrCodeE
         */
        ErrCodeE Set(const char *oemId);

    private:
        char m_oemId[MaxOemIdSize];
    };

}

#endif //EMX_OemId_HPP
