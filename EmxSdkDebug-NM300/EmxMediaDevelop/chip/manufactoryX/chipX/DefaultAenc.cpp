//
// Created by xiong on 2020/11/19.
//
#include "DefaultAenc.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

ErrCodeE DefaultAenc::OnCreate() {
    //! 加载需要的参数
    Json::Value root;
    MediaServerCtx::GetInst()->GetCfg(m_module->m_name.c_str(), root);
    auto &json = root[m_chn]["attr"];
    m_aiDev = json["AiDev"].asInt();
    m_aiChn = json["AiChn"].asInt();
    m_aencChn = json["AencChn"].asInt();
    m_gainMin = json["gain"]["min"].asInt();
    m_gainMax = json["gain"]["max"].asInt();
    m_aioSampleRate = json["aioSampleRate"].asInt();
    m_frameSize = json["frameSize"].asInt();
    m_vqeType = json["vqeType"].asInt();
    //! OnCreate阶段使用m_param来创建
    m_pParam = &m_param;
    if (CreateAenc() != ErrCodeE::Success) {
        return ErrCodeE::Failure;
    }
    StartStream();
    emxlogi("chn %d done\n", m_chn);
    return ErrCodeE::Success;
}

void DefaultAenc::OnDestroy() {
    m_pParam = &m_param;
    StopStream();
    DestroyAenc();
    emxlogd("chn %d done\n", m_chn);
}

ErrCodeE DefaultAenc::OnParamChanged() {
    m_pParam = &m_param;
    StopStream();
    DestroyAenc();
    m_pParam = &m_paramNew;
    if (CreateAenc() != ErrCodeE::Success) {
        return ErrCodeE::Failure;
    }
    StartStream();
    m_pParam = &m_param;
    return ErrCodeE::Success;
}

ErrCodeE DefaultAenc::GetOneFrame(FrameData &frame) {
//    int s32Ret;
    //! 注意这里的m_stFrame需要保证持续有效，最好做成成员变量，以便在ReleaseOneFrame中释放
//    s32Ret = ChipAinGetFrame(m_aiDev, m_aiChn, &m_stFrame, &m_stAecFrm, 1000);
//    if (s32Ret != 0) {
//        emxloge("chn %d ChipAinGetFrame failed with %#x!!\n", m_chn, s32Ret);
//        return ErrCodeE::Failure;
//    } else {
    //! 填充frame
//        m_got = true;
//        frame.data.data = m_stFrame.u64VirAddr[0];
    //! 注意这里是字节数
//        frame.data.size = (int) m_stFrame.u32Len << 1;
//        frame.tsInUs = m_stFrame.u64TimeStamp;
//    }
    return ErrCodeE::Success;
}

void DefaultAenc::ReleaseOneFrame() {
    //! 释放m_stFrame
//    if (m_got) {
//        ChipAinReleaseFrame(m_aiDev, m_aiChn, &m_stFrame, &m_stAecFrm);
//        m_got = false;
//    }
}

ErrCodeE DefaultAenc::SetVolume(int32_t volume) {
    if (volume == m_pParam->volume && !m_pParam->mute)
        return ErrCodeE::Success;
    if(m_pParam->mute){
//        ChipAinSetMute(m_aiDev,false);
    }
    //! volume的取值范围是0-100，这里需要线性映射到芯片实际的增益值范围
    int gain = m_gainMin + (m_gainMax - m_gainMin) * volume / 100;
    emxlogd("set volume[%d] = gain[%d]\n", volume, gain);
//    ChipAinSetVolume(m_aiDev, gain);
    return ErrCodeE::Success;
}

ErrCodeE DefaultAenc::Mute(bool ena) {
    if (ena) {
//        ChipAinSetMute(m_aiDev,true);
        emxlogd("ai mute\n");
    } else {
//        ChipAinSetMute(m_aiDev,false);
        //! volume的取值范围是0-100，这里需要线性映射到芯片实际的增益值范围
        int gain = m_gainMin + (m_gainMax - m_gainMin) * m_pParam->volume / 100;
//        ChipAinSetVolume(m_aiDev, gain);
        emxlogd("set volume[%d] = gain[%d]\n", m_pParam->volume, gain);
    }
    return ErrCodeE::Success;
}

ErrCodeE DefaultAenc::CreateAenc() {
    //! 这里面用到的参数统一使用m_pParam即可
    //! 配置编码器
//    int s32Ret = 0;
//    s32Ret = ChipAencCreateChn(m_aiChn);
//    if (s32Ret != 0) {
//        emxloge("chn %d ChipAencCreateChn failed!ret=%#x\n", m_chn, s32Ret);
//        return ErrCodeE::Failure;
//    }
    //! 这里当用户配置的采样率与本地实际的采样率不匹配时，使能重采样(如果不方便更改Ain采样率的话)
//    s32Ret = ChipAencStartChn(m_aiChn, m_aioSampleRate != m_pParam->sampleRate);
//    if (s32Ret != 0) {
//        emxloge("chn %d ChipAencStartChn failed!ret=%#x\n", m_chn, s32Ret);
//        return ErrCodeE::Failure;
//    }
    //! 初始化音量
    SetVolume(m_pParam->volume);
    Mute(m_pParam->mute);
    return ErrCodeE::Success;
}

void DefaultAenc::DestroyAenc() {
    //! 销毁通道
//    ChipVencDestroyChn(m_aiDev, m_aiChn, m_aioSampleRate != m_pParam->sampleRate, m_vqeType);
}
