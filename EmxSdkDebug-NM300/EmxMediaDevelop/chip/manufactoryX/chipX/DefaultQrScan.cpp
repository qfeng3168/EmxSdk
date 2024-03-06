//
// Created by xiong on 2023/9/6.
//

#include "DefaultQrScan.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

ErrCodeE DefaultQrScan::OnCreate() {
    //! 加载需要的参数
//    Json::Value root;
//    MediaServerCtx::GetInst()->GetCfg(m_module->m_name.c_str(), root);
//    auto &json = root[m_chn]["attr"];
//    m_vpssGrp = (VPSS_GRP) json["VpssGrp"].asInt();
//    m_vpssChn = (VPSS_CHN) json["VpssChn"].asInt();
    //! 注意这里需要提供一下宽高信息GetChnImgSize里面会用
//    VPSS_CHN_ATTR_S stChnAttr = {};
//    int s32Ret = ChipVpssGetChnAttr(m_vpssGrp, m_vpssChn, &stChnAttr);
//    if (0 != s32Ret) {
//        emxloge("chn %d ChipVpssGetChnAttr failed!\n", m_chn);
//        return ErrCodeE::Failure;
//    }
//    m_imgSize.w = (int) stChnAttr.u32Width;
//    m_imgSize.h = (int) stChnAttr.u32Height;
    //! 必要的初始化
//    m_mem = open("/dev/mem", O_CREAT | O_RDWR | O_SYNC);
//    if (m_mem < 0) {
//        emxloge("chn %d open /dev/mem failed\n", m_chn);
//        return ErrCodeE::Failure;
//    }
    return ErrCodeE::Success;
}

void DefaultQrScan::OnDestroy() {
    //! 销毁操作
//    close(m_mem);
}

Size DefaultQrScan::GetChnImgSize() {
    //! Initialize中提供的宽高信息
    return m_imgSize;
}

ErrCodeE DefaultQrScan::GetOneImg(uint8_t **dataY) {
    //! 注意这里的m_stFrame需要保证持续有效，最好做成成员变量，以便在ReleaseOneImg中释放
//    int s32Ret = ChipVpssGetChnFrame(m_vpssGrp, m_vpssChn, &m_stFrame, 200);
//    if (0 != s32Ret) {
//        emxloge("chn %d ChipVpssGetChnFrame failed!\n", m_chn);
//        return ErrCodeE::Failure;
//    }
//    Chip_U64 phyAddr = m_stFrame.stVFrame.u64PhyAddr[0];
//    *dataY = (uint8_t *) Chip_SYS_Mmap(phyAddr, m_stFrame.stVFrame.u32Length[0]);
//    if (!*dataY) {
//        ChipVpssReleaseChnFrame(m_vpssGrp, m_vpssChn, &m_stFrame);
//        emxloge("chn %d dataY == nullptr!\n", m_chn);
//        return ErrCodeE::Failure;
//    }
    return ErrCodeE::Success;
}

void DefaultQrScan::ReleaseOneImg(uint8_t *dataY) {
    //! 释放m_stFrame
//    Chip_SYS_Munmap(dataY, m_stFrame.stVFrame.u32Length[0]);
//    /* release frame after using */
//    ChipVpssReleaseChnFrame(m_vpssGrp, m_vpssChn, &m_stFrame);
}
