#ifndef EMX_LanguageVer_HPP
#define EMX_LanguageVer_HPP

#include "EmxCore.hpp"

namespace Emx {

    //!@brief 提供设备语言版本配置和获取
    class LanguageVer {
    public:
        //!@brief 语言类型定义
        enum class Type : int32_t {
            Unknown = -1,   //!< 未知   
            Chinese,        //!< 中文
            English,        //!< 英文
            Spanish,        //!< 西班牙文
            French,         //!< 法文
            German,         //!< 德文
            Japanese,       //!< 日文
            Korean,         //!< 韩文
            Arabic,         //!< 阿拉伯文
            Russian,        //!< 俄文
            Portuguese,     //!< 葡萄牙文
            Italian,        //!< 意大利文
            Dutch,          //!< 荷兰文
            Hindi,          //!< 印地文
            Bengali,        //!< 孟加拉文
            Turkish,        //!< 土耳其文
            Vietnamese,     //!< 越南文
            Thai,           //!< 泰文
            Polish,         //!< 波兰文
            Indonesian,     //!< 印尼文
            Greek           //!< 希腊文
        };
    public:
        LanguageVer() {
            m_type = Type::Unknown;
        }

        /**
         * @brief 从flash中加载参数
         * @return ErrCodeE
         */
        ErrCodeE Load();

        /**
         * @brief 获取语音类型
         * @return 语言类型
         */
        Type Get() { return m_type; }

        /**
         * @brief 设置SN
         * @param sn [in] SN
         * @return ErrCodeE
         */
        ErrCodeE Set(const Type &type);

    private: 
        Type m_type;
    };

}

#endif //EMX_LanguageVer_HPP
