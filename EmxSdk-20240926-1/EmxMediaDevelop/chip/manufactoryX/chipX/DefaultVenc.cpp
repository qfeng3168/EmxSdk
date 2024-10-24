//
// Created by xiong on 2020/10/27.
//
#include "DefaultVenc.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

ErrCodeE DefaultVenc::OnCreate() {
    //! 加载需要的参数
    Json::Value root;
    MediaServerCtx::GetInst()->GetCfg(m_module->m_name.c_str(), root);
    auto &json = root[m_chn]["attr"];
    m_vpssGrp = json["VpssGrp"].asInt();
    m_vpssChn = json["VpssChn"].asInt();
    m_vencChn = json["VencChn"].asInt();

    //! OnCreate阶段使用m_param来创建
    m_pParam = &m_param;
    if (CreateVenc() != ErrCodeE::Success) {
        return ErrCodeE::Failure;
    }
    StartStream();
    m_paramChange = true;
    emxlogi("chn %d done\n", m_chn);
    return ErrCodeE::Success;
}

void DefaultVenc::OnDestroy() {
    m_pParam = &m_param;
    StopStream();
    DestroyVenc();
    emxlogd("chn %d done\n", m_chn);
}

ErrCodeE DefaultVenc::OnParamChanged() {
    //! OnParamChanged中停止阶段使用m_param来销毁
    m_pParam = &m_param;
    StopStream();
    DestroyVenc();
    //! OnParamChanged中启动阶段使用m_paramNew来创建
    m_pParam = &m_paramNew;
    if (CreateVenc() != ErrCodeE::Success) {
        return ErrCodeE::Failure;
    }
    StartStream();
    //! 重新配置编码参数后，设置参数改变标志位
    m_paramChange = true;
    m_pParam = &m_param;
    return ErrCodeE::Success;
}

ErrCodeE DefaultVenc::FlushIDR() {
    //! 这里刷新相应通道的I帧
//    ChipVencRequestIDR(m_vencChn);
    return ErrCodeE::Success;
}

ErrCodeE DefaultVenc::GetOneFrame(FrameData &frame) {
    //! 这里面需要获取一帧编码数据，并填充frame
//    int s32Ret;
//    s32Ret = ChipVencQueryStatus(m_vencChn, &stStat);
//    if (0 != s32Ret) {
//        emxloge("chn %d ChipVencQueryStatus failed with %#x!\n", m_chn, s32Ret);
//        return ErrCodeE::Failure;
//    }
//
//    if (0 == stStat.u32CurPacks) {
//        emxloge("chn %d NOTE: Current  frame is NULL!\n", m_chn);
//        return ErrCodeE::Failure;
//    }
//
    //! 注意这里的m_stream需要保证持续有效，最好做成成员变量，以便在ReleaseOneFrame中释放
//    m_stream.pstPack = (VENC_PACK_S *) EP_MALLOC(sizeof(VENC_PACK_S) * stStat.u32CurPacks);
//    if (nullptr == m_stream.pstPack) {
//        emxloge("chn %d malloc stream pack failed!\n", m_chn);
//        return ErrCodeE::Failure;
//    }
//
//    s32Ret = ChipVencGetStream(m_vencChn, &m_stream, 1000);
//    if (0 != s32Ret) {
//        free(m_stream.pstPack);
//        m_stream.pstPack = nullptr;
//        emxloge("chn %d ChipVencGetStream failed with %#x!\n", m_chn, s32Ret);
//        return ErrCodeE::Failure;
//    }
    //! 填充frame
//    frame.isKeyFrame = false;
//    for (uint32_t i = 0; i < m_stream.u32PackCount; i++) {
//        EmxData d = {};
//        d.data = m_stream.pstPack[i].pu8Addr + m_stream.pstPack[i].u32Offset;
//        d.size = (int) (m_stream.pstPack[i].u32Len - m_stream.pstPack[i].u32Offset);
//        frame.data.push_back(d);
//        unsigned char nalType = m_stream.pstPack[i].pu8Addr[m_stream.pstPack[i].u32Offset + 4];
//        /*vps sps pps sei IDR*/
    //! 这里I帧的判断根据芯片厂商实际情况来，不一定是0x67/0x40
//        if ((nalType == 0x67) || (nalType == 0x40)) {
//            frame.isKeyFrame = true;
//        }
//    }
    //! 这里需要将m_paramChange赋值给frame，并复位m_paramChange
//    frame.isParamChanged = m_paramChange;
//    if (m_paramChange) {
//        m_paramChange = false;
//    }
    //! m_seq需要每次+1
//    frame.seq = m_seq++;
    //! 注意这里的ts是64位微妙级时间戳
//    frame.tsInUs = m_stream.pstPack[0].u64PTS;
    return ErrCodeE::Success;
}

void DefaultVenc::ReleaseOneFrame() {
    //! 释放m_stream
//    if (m_stream.pstPack) {
//        ChipVencReleaseStream(m_vencChn, &m_stream);
//        free(m_stream.pstPack);
//        m_stream.pstPack = nullptr;
//    }
}

ErrCodeE DefaultVenc::CreateVenc() {
    //! 这里面用到的参数统一使用m_pParam即可
    //! 配置编码器
//    int ret = ChipVencCreateChn(m_vencChn, m_pParam->codec, m_pParam->width, m_pParam->height);
//    if (ret != 0) {
//        ChipVencDestroyChn(m_vencChn);
//        emxloge("chn %d ChipVencCreateChn failed with %#x!\n", m_chn, ret);
//        return ErrCodeE::Failure;
//    }

    //! 启动接收
//    ret = ChipVencStartRecvChn(m_vencChn);
//    if (ret != 0) {
//        ChipVencDestroyChn(m_vencChn);
//        emxloge("chn %d ChipVencStartRecvChn failed with %#x!\n", m_chn, ret);
//        return ErrCodeE::Failure;
//    }
    return ErrCodeE::Success;
}

void DefaultVenc::DestroyVenc() {
    //! 销毁通道
//    ChipVencDestroyChn(m_viChn);
}

