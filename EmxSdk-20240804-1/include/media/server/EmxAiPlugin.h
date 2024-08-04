//
// Created by xiong on 2024/03/19.
//

#ifndef EMX_EmxAiPlugin_HPP
#define EMX_EmxAiPlugin_HPP
#include "stdint.h"
#ifdef __cplusplus
extern "C" {
#define EMX_AI_PLUGIN_API(ret, name, param)                                                                            \
    typedef ret(*name##FuncPtr) param;                                                                                 \
    ret name param;

struct EmxAiPluginVersionT {
    const char *sEmxAiPluginVersion;
    const char *sEmxMediaVersion;
    const char *sEmxSdkVersion;
    const char *sChipManufactory;
    const char *sChipSerial;
    const char *sChipAiVersion;
    const char *sChipSdkVersion;
    const char *sAiPluginProfile;
};
/**
 * @brief 打印AiPlugin相关版本信息
 * @return 0表示成功，其他表示失败
 */
EMX_AI_PLUGIN_API(EmxAiPluginVersionT, EmxAiPluginGetVersion, ())

/**
 * @brief 初始化AiPlugin
 * @param path [in] AiPlugin的安装路径
 * @return 0表示成功，其他表示失败
 */
EMX_AI_PLUGIN_API(int, EmxAiPluginInit, (const char *path))

/**
 * @brief 反初始化AiPlugin
 */
EMX_AI_PLUGIN_API(void, EmxAiPluginDeInit, ())

/**
 * @brief 创建Ai通道
 * @param chn [in] 通道号
 * @return 0表示成功，其他表示失败
 */
EMX_AI_PLUGIN_API(void *, EmxAiPluginMakeAiChn, (int32_t chn))
}
#endif//__cplusplus
#endif//EMX_EmxAiPlugin_HPP