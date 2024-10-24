//
// Created by xiong on 2023/9/6.
//

#include "DefaultSnap.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

ErrCodeE DefaultSnap::OnCreate() {
    //! 加载需要的参数
    Json::Value root;
    MediaServerCtx::GetInst()->GetCfg(m_module->m_name.c_str(), root);
    auto &json = root[m_chn];
    m_vpssGrp = json["VpssGrp"].asInt();
    m_vpssChn = json["VpssChn"].asInt();
    m_vencChn = json["VencChn"].asInt();

    //! 初始化编码通道
//    s32Ret = ChipVencCreateChn(m_vencChn);
//    if (s32Ret != 0) {
//        emxloge("chn %d ChipVencCreateChn [%d] failed with %#x\n", m_chn, m_vencChn, s32Ret);
//        return ErrCodeE::Failure;
//    }
    //! 启动接收
//    s32Ret = ChipVencStartRecvFrame(m_vencChn);
//    if (0 != s32Ret) {
//        emxloge("chn %d ChipVencStartRecvFrame [%d] failed with %#x\n", m_chn, m_vencChn, s32Ret);
//        ChipVencDestroyChn(m_vencChn);
//        return ErrCodeE::Failure;
//    }
    emxlogi("chn %d done\n", m_chn);
    return ErrCodeE::Success;
}

void DefaultSnap::OnDestroy() {
    //! 销毁通道
//    ChipVencDestroyChn(m_viChn);
    emxlogi("chn %d done\n", m_chn);
}

ErrCodeE DefaultSnap::GetOneSnap(std::vector <EmxData> &data) {
    //! 这里面需要获取一帧编码数据，并填充data
//    int s32Ret;
//    VENC_CHN_STATUS_S stStat = {};
//
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
    //! 注意这里的m_stream需要保证持续有效，最好做成成员变量，以便在ReleaseOneSnap中释放
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
//        emxloge("chn %d HI_MPI_VENC_GetStream failed with %#x!\n", m_chn, s32Ret);
//        return ErrCodeE::Failure;
//    }
    //! 填充data
//    for (uint32_t i = 0; i < m_stream.u32PackCount; i++) {
//        EmxData d = {};
//        d.data = m_stream.pstPack[i].pu8Addr + m_stream.pstPack[i].u32Offset;
//        d.size = (int) (m_stream.pstPack[i].u32Len - m_stream.pstPack[i].u32Offset);
//        data.push_back(d);
//    }
    return ErrCodeE::Success;
}

void DefaultSnap::ReleaseOneSnap() {
    //! 释放m_stream
//    if (m_stream.pstPack) {
//        ChipVencReleaseStream(m_vencChn, &m_stream);
//        free(m_stream.pstPack);
//    }
}