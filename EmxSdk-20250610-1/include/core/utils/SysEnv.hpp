//
// Created by xiong on 2023/9/6.
//

#ifndef EMX_SysEnv_HPP
#define EMX_SysEnv_HPP

#include "core/EmxTypeDef.hpp"

namespace Emx {
    /** @addtogroup utils
     *  @{
     */

    class SysEnv {
    public:
        static const char *GetEnv(const char *name);
        /*!
         * @brief 获取可读写文件存储的根目录EMX_DATA_DIR
         * @return 用于存储一般可读写文件的目录
         */
        static const char *GetDataDir(){return GetEnv("EMX_DATA_DIR");}

        /*!
         * @brief 获取固件根目录EMX_FIRMWARE_DIR
         * @return 固件根目录
         */
        static const char *GetFirmwareDir(){return GetEnv("EMX_FIRMWARE_DIR");}

        /*!
         * @brief 获取第三方程序库目录EMX_3DR_DIR
         * @return 第三方程序库目录
         */
        static const char *Get3drDir(){return GetEnv("EMX_3DR_DIR");}

        /*!
         * @brief 获取应用目录EMX_APP_DIR
         * @return 应用目录
         */
        static const char *GetAppDir(){return GetEnv("EMX_APP_DIR");}

        /*!
         * @brief 获取媒体目录EMX_MEDIA_DIR
         * @return 媒体目录
         */
        static const char *GetMediaDir(){return GetEnv("EMX_MEDIA_DIR");}

        /*!
         * @brief 获取配置文件根目录EMX_CONFIGS_DIR
         * @return 配置文件根目录
         */
        static const char *GetConfigsDir(){return GetEnv("EMX_CONFIGS_DIR");}

        /*!
         * @brief 获取参数文件根目录EMX_PARAM_DIR
         * @return 参数文件根目录
         */
        static const char *GetParamDir(){return GetEnv("EMX_PARAM_DIR");}

        /**
         * @brief 通过文件的相对路径，获取文件绝对路径
         * 会首先尝试判断以可读写分区EMX_DATA_DIR为根路径下，是否存在此文件
         * 如果存在则优先使用，主要是为了静态配置文件或资源文件的动态烧写
         * @param buffer [in,out] 用于存储返回的绝对路径，长度需要是EMX_MAX_PATH_SIZE
         * @param fmt [in] 格式化参数
         * @param ... [in] 可变参数,应用文件的相对路径，例如"net/dns.script"
         * @return 绝对路径
         */
        static const char *GetCfgFilePath(char buffer[EMX_MAX_PATH_SIZE], const char *fmt, ...);

        /**
         * @brief 通过文件的相对路径，获取文件绝对路径
         * 会首先尝试判断以可读写分区EMX_DATA_DIR为根路径下，是否存在此文件
         * 如果存在则优先使用，主要是为了静态配置文件或资源文件的动态烧写
         * @param fmt [in] 格式化参数
         * @param ... [in] 可变参数,应用文件的相对路径，例如"net/dns.script"
         * @return 绝对路径
         */
        static std::string GetCfgFilePath(const char *fmt, ...);
    };
    /** @}*/
}

#endif //EMX_SysEnv_HPP
