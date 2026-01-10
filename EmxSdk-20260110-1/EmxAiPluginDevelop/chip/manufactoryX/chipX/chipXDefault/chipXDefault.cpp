//
// Created by xiong on 2022/4/27.
//
#include "chipXDefault.hpp"
#include "MotionDetection.hpp"
#include "PersonDetection.hpp"
#include "CrossingInvasionComm.hpp"
#include "FaceAlgorithm.hpp"
#include "PassengerFlowStatistics.hpp"

using namespace Emx;

static char g_AiPluginWorkspace[EMX_MAX_PATH_SIZE] = {0};
static EmxAiPluginVersionT g_AiPluginVersion;
int EmxAiPluginInit(const char *path) {
    //! 这里进行整个插件的初始化

    //! 加载本地配置
    strncpy(g_AiPluginWorkspace, path, sizeof(g_AiPluginWorkspace));
    
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
    char tmp[EMX_MAX_PATH_SIZE] = {0};
    if (LoadParam(param) != ErrCodeE::Success) return ErrCodeE::Failure;

    sprintf(tmp, "%s/configs/%s", g_AiPluginWorkspace, "config.json");
    Json::Value node,root;
    if (EasyJson::Load(tmp, node) != ErrCodeE::Success) {
        emxloge("load %s failed\n", tmp);
        return ErrCodeE::ParseFailed;
    }
    if (node.isArray()) {
        root = node[m_chn];
    }else{
        root = node;
    }
    //! 根据"chnAttr"数组确定插件的通道数
    if(root.isMember("MotionDetection") && root["MotionDetection"].asBool()){
        m_algs.push_back(new MotionDetection(m_chn));
        emxlogi("Config MotionDetection mode start!\n");
    }

    if(root.isMember("PersonDetection") && root["PersonDetection"].asBool()){
        m_algs.push_back(new PersonDetection(m_chn));
        emxlogi("Config PersonDetection mode start!\n");
    }
    
    if(root.isMember("CrossingInvasionComm") && root["CrossingInvasionComm"].asBool()){
        m_algs.push_back(new CrossingInvasionComm(m_chn));
        emxlogi("Config CrossingInvasionComm mode start!\n");
    }

    if(root.isMember("PassengerFlowStatistics") && root["PassengerFlowStatistics"].asBool()){
        m_algs.push_back(new PassengerFlowStatistics(m_chn));
        emxlogi("Config PassengerFlowStatistics mode start!\n");
    }

    if(root.isMember("FaceAlgorithm") && root["FaceAlgorithm"].asBool()){
        m_algs.push_back(new FaceAlgorithm(m_chn));
        emxlogi("Config FaceAlgorithm mode start!\n");
    }
    
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