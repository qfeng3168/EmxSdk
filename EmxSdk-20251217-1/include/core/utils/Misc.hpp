//
// Created by xiong on 2022/6/9.
//

#ifndef EMX_Misc_HPP
#define EMX_Misc_HPP

#include "core/EmxTypeDef.hpp"
#include "uv.h"

namespace Emx {
    /** @addtogroup utils
     *  @{
     */

    //!未分类的各种操作接口
    class Misc {
    public:
        /*!
         * @brief 绑定当前线程到指定CPU核心
         * @param num [in] CPU核心
         */
        static void BindCpu(int num);

        /*!
         * @brief 获取当前进程物理内存使用情况
         * @return 物理内存，单位字节
         */
        static uint32_t GetRSS();

        /*!
         * @brief 获取进程资源使用情况
         * @param usage [out] 资源使用情况
         */
        static void GetResUsage(uv_rusage_t &usage);

        //!系统资源使用情况
        struct SysUsage {
            int cpu;//!<0-100, cpu占有率
            int memLoad;//!<占用内存(KB)
            int memFree;//!<剩余内存(KB)
            int memTotal;//!<内存总量(KB)
        };

        /*!
         * @brief 获取系统资源使用情况
         * @param usage [out] 资源使用情况
         * @param calcTimeMs [in] 统计计算时间ms
         */
        static ErrCodeE GetSysUsage(SysUsage &usage, int calcTimeMs);

        /*!
         * @brief 根据名字检查对应的进程是否存在
         * @param app [in]进程明
         * @return 是否存在
         */
        static bool ApplicationExist(const char *app);

        //!kill掉与本进程同名的进程
        static void KillBeforeRun();

        /*!
         * @brief kill掉与本进程同名的进程
         * @param cmd 进程名
         */
        static void KillBeforeRun(const char *cmd);

        /*!
         * @brief 整型转字节数组
         * @param num [in] 输入整型数
         * @param array [out] 输出字节数组
         */
        static void UIntToArray(uint32_t num, uint8_t *array);

        /*!
         * @brief 字节数组转整型
         * @param array [in]输入字节数组
         * @return 返回整型
         */
        static uint32_t ArrayToUint32(uint8_t *array);

        /*!
         * @brief 根据范围获取随机数
         * @param min [in]最小值
         * @param max [in]最大值
         * @return 随机数
         */
        static int GetRandom(int min = 0, int max = INT_MAX);

        static const char *RebootTopic;

        /*!
         * @brief 根据ip和port生成ipv4 sock 地址
         * @param ip [in]ipv4地址
         * @param port [in]端口
         * @param addr [out]sock地址
         */
        static void MakeAddr4(const char *ip, int port, struct sockaddr_in *addr) {
            uv_ip4_addr(ip, port, addr);
        }

        /*!
         * @brief 根据ip和port生成ipv6 sock 地址
         * @param ip [in]ipv6地址
         * @param port [in]端口
         * @param addr [out]sock地址
         */
        static void MakeAddr6(const char *ip, int port, struct sockaddr_in6 *addr) {
            uv_ip6_addr(ip, port, addr);
        }
    };
    /** @}*/
}

#endif //EMX_Misc_HPP
