//
// Created by xiong on 2020/11/23.
//
#include "DefaultAdec.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

ErrCodeE DefaultAdec::OnCreate() {
    //! 加载需要的参数
    Json::Value root;
    MediaServerCtx::GetInst()->GetCfg(m_module->m_name.c_str(), root);
    auto &json = root[m_chn]["attr"];
    m_aoDev = json["AoDev"].asInt();
    m_aoChn = json["AoChn"].asInt();
    m_adecChn = json["AdecChn"].asInt();
    m_gainMin = json["gain"]["min"].asInt();
    m_gainMax = json["gain"]["max"].asInt();
    m_aioSampleRate = json["aioSampleRate"].asInt();
    m_frameSize = json["frameSize"].asInt();
    //! OnCreate阶段使用m_param来创建
    m_pParam = &m_param;
    if (CreateAdec() != ErrCodeE::Success) {
        return ErrCodeE::Failure;
    }
    emxlogi("chn %d done\n", m_chn);
    return ErrCodeE::Success;
}

void DefaultAdec::OnDestroy() {
    m_pParam = &m_param;
    DestroyAdec();
    emxlogd("chn %d done\n", m_chn);
}

ErrCodeE DefaultAdec::OnParamChanged() {
    m_pParam = &m_param;
    DestroyAdec();
    m_pParam = &m_paramNew;
    auto e = CreateAdec();
    m_pParam = &m_param;
    return e;
}

ErrCodeE DefaultAdec::SetVolume(int32_t volume) {
    //! volume的取值范围是0-100，这里需要线性映射到芯片实际的增益值范围
//    int gainTarget = m_gainMin + (m_gainMax - m_gainMin) * volume / 100;
//    int gainOrg = -1;
//    ChipAoGetVolume(m_aoDev, &gainOrg);
//    if (gainOrg != gainTarget) {
//        emxlogd("set volume[%d] / gain[%d]\n", volume, gainTarget);
//        ChipAoSetVolume(m_aoDev, gainTarget);
//    }
    return ErrCodeE::Success;
}

void DefaultAdec::SendToAo(const int16_t *pcm, int32_t pktNum) {
    //! 播放声音，注意这里的pktNum指的是pcm采样点数，不是字节数
//    int off = 0;
//    while (pktNum > 0) {
//        AUDIO_FRAME_S audioFrame = {};
//        int sendNum = pktNum > m_frameSize ? m_frameSize : pktNum;
//        audioFrame.u64VirAddr[0] = (Chip_U8 * )(pcm + off);
//        audioFrame.u32Len = sendNum;//samples size for each channel
//        audioFrame.u64TimeStamp = 0;
//        audioFrame.enSoundmode = m_soundMode;
//        audioFrame.enBitwidth = m_bitWidth;
//        int ret = ChipAoSendFrame(m_aoDev, m_aoChn, &audioFrame, 1000);
//        if (ret != 0) {
//            emxloge("chn %d send Frame Data error with %#x,sendNum=%d\n", m_chn, ret, sendNum);
//        }
//        off += sendNum;
//        pktNum -= sendNum;
//    }
}

MediaInfAdec::PlayStatusE DefaultAdec::GetPlayStatus() {
    //! 获取当前是否有声音正在播放
//    AO_CHN_STATE_S status = {};
//    int ret = ChipAoQueryChnStat(m_aoDev, m_aoChn, &status);
//    if (ret != 0) {
//        emxloge("chn %d ChipAoQueryChnStat failed with %#x\n", m_chn, ret);
//        return Unknown;
//    } else {
//        return status.u32ChnBusyNum < (uint32_t) m_frameSize * 2 ? Finish : Playing;
//    }
    return Unknown;
}

ErrCodeE DefaultAdec::CreateAdec() {
    //! 初始化通道
//    int s32Ret = 0;
//    s32Ret = ChipAoCreateChn(m_aoChn);
//    if (s32Ret != 0) {
//        emxloge("chn %d ChipAoCreateChn failed!ret=%#x\n", m_chn, s32Ret);
//        return ErrCodeE::Failure;
//    }
    //! 初始化音量
    SetVolume(m_pParam->volume);
    return ErrCodeE::Success;
}

void DefaultAdec::DestroyAdec() {
    //! 销毁通道
//    ChipAdecDestroyChn(m_aoChn);
}

void DefaultAdec::PadSilentPcm() {
    auto pcm = malloc(m_frameSize * 2);
    if (!pcm) {
        emxloge("malloc pcm %d size failed\n", m_frameSize * 2);
        return;
    }
    memset(pcm, 0, m_frameSize * 2);
    //pad silent pcm
    int cnt = m_silentTimeMs * m_aioSampleRate / 1000 / m_frameSize;
    emxlogt("chn %d pad %d ms (cnt=%d) silent pcm\n", m_chn, m_silentTimeMs, cnt);
    while (cnt--) { SendToAo((const int16_t *) pcm, m_frameSize); }
    free(pcm);
}
