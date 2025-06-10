#ifndef EMX_Language_HPP
#define EMX_Language_HPP

#include "EmxCore.hpp"

namespace Emx {

    //!@brief 提供设备语言版本配置和获取
    class Language {
    public:
        Language() { memset(m_language, 0, sizeof(m_language)); }

        static const int MaxLanguageSize = 128;//!<Language字符串最大大小

        /**
         * @brief 从flash中加载参数
         * @return ErrCodeE
         */
        ErrCodeE Load();

        /**
         * @brief 获取语种类型
         * @return 语种类型
         */
        char *Get() { return m_language; }

        /**
         * @brief 设置语种类型
         * @param type [in] 语种类型
         * @return ErrCodeE
         */
        ErrCodeE Set(const char *language);

    private:
        char m_language[MaxLanguageSize];
    };

}// namespace Emx

#endif//EMX_Language_HPP
