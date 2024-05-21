//
// Created by xiong on 2022/4/27.
//
#include "chipXDefault.hpp"
#include "FdPdFusion.hpp"
#include "MotionDetection.hpp"
#include "PersonDetection.hpp"
using namespace Emx;

static char g_AiPluginWorkspace[EMX_MAX_PATH_SIZE] = {0};
static EmxAiPluginVersionT g_AiPluginVersion;
int EmxAiPluginInit(const char *path) {
    //! 这里进行整个插件的初始化

    //! 加载本地配置
    strncpy(g_AiPluginWorkspace, path, sizeof(g_AiPluginWorkspace));
    char tmp[EMX_MAX_PATH_SIZE] = {0};
    sprintf(tmp, "%s/configs/%s", g_AiPluginWorkspace, "config.json");
    Json::Value root;
    if (EasyJson::Load(tmp, root) != ErrCodeE::Success) {
        emxloge("load %s failed\n", tmp);
        return -1;
    }
    //! 根据"chnAttr"数组确定插件的通道数

    //! 根据芯片，进行其他必要的操作，例如某些全局的初始化或者驱动的加载等
    return 0;
}

void EmxAiPluginDeInit() {
    //! 这里进行整个插件的反初始化，上层会将所有的通道实例调用OnDestroy后进行delete，然后调用此接口
}

void *EmxAiPluginMakeAiChn(int32_t chn) {
    //! 创建通道实例，上层调用EmxAiPluginInit后会根据通道数要求返回相应的实例
    return (void *) new chipXDefault(chn);
}


ErrCodeE chipXDefault::OnCreate() {
    //! 这里进行AI插件通道的创建
    Json::Value param;
    if (LoadParam(param) != ErrCodeE::Success) return ErrCodeE::Failure;
    m_algs.push_back(new MotionDetection(m_chn));
    m_algs.push_back(new PersonDetection(m_chn));
    m_algs.push_back(new FdPdFusion(m_chn));
    for (auto &e: m_algs) { e->Create(g_AiPluginWorkspace, param[m_chn]); }
    return ErrCodeE::Success;
}

void chipXDefault::OnDestroy() {
    //! 这里进行AI插件通道的销毁
    for (auto &e: m_algs) {
        e->Destroy();
        delete e;
    }
    m_algs.clear();
}

struct EmxAiPluginVersionT EmxAiPluginGetVersion() {
    g_AiPluginVersion.sEmxAiPluginVersion = EmxAiPluginVersion;
    g_AiPluginVersion.sEmxMediaVersion = EmxMediaVersion;
    g_AiPluginVersion.sEmxSdkVersion = EmxSdkVersion;
    g_AiPluginVersion.sChipManufactory = ChipManufactory;
    g_AiPluginVersion.sChipSerial = ChipSerial;
    g_AiPluginVersion.sChipAiVersion = ChipAiVersion;
    g_AiPluginVersion.sChipSdkVersion = ChipSdkVersion;
    g_AiPluginVersion.sAiPluginProfile = AiPluginProfile;
    return g_AiPluginVersion;
}