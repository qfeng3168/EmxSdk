//
// Created by xiong on 2021/8/10.
//
#include "DefaultOsd.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

ErrCodeE DefaultOsd::OnCreate() {
    //! 加载需要的参数
    /*    Json::Value root;
    MediaServerCtx::GetInst()->GetCfg(m_module->m_name.c_str(), root);
    auto &json = root[m_chn]["attr"];
    m_vpssGrp = (VPSS_GRP) json["VpssGrp"].asInt();
    m_vpssChn = (VPSS_CHN) json["VpssChn"].asInt();*/
    //! 注意这里需要提供一下宽高信息GetChnPicSize里面会用
    //    int s32Ret;
    //    VPSS_CHN_ATTR_S stChnAttr = {};
    //    s32Ret = ChipVpssGetChnAttr(m_vpssGrp, m_vpssChn, &stChnAttr);
    //    if (s32Ret != 0) {
    //        emxloge("ChipVpssGetChnAttr error ret=%#x !\n", s32Ret);
    //        return ErrCodeE::Failure;
    //    }
    //    m_chnWidth = (int) stChnAttr.u32Width;
    //    m_chnHeight = (int) stChnAttr.u32Height;
    m_osdIdx = 0;
    return ErrCodeE::Success;
}

void DefaultOsd::OnDestroy() {
    //! 销毁操作
}

Size DefaultOsd::GetChnPicSize() {
    //! Initialize中提供的宽高信息
    return {m_chnWidth, m_chnHeight};
}

void DefaultOsd::RegionCreateAll(std::list<RegionInfo> &region) {
    //! 创建所有region，厂商按照自己的特性来，最终需要填充region中的id字段
    //! 需要自己保证id的唯一性即可
    // int id = 0;
    // for (auto &e: region) {
    // e.id = id++;
    //    int s32Ret;
    //    RGN_ATTR_S stRgnAttr = {};
    //    RGN_CHN_ATTR_S stChnAttr = {};
    //    MMF_CHN_S stChn = {};
    //    stChn.enModId = Chip_ID_VPSS;
    //    stChn.s32DevId = m_vpssGrp;
    //    stChn.s32ChnId = m_vpssChn;
    //
    //    //OverlayEx
    //    stRgnAttr.unAttr.stOverlayEx.stSize.u32Width = e.w;
    //    stRgnAttr.unAttr.stOverlayEx.stSize.u32Height = e.h;
    //    stRgnAttr.unAttr.stOverlayEx.enPixelFormat = PIXEL_FORMAT_ARGB_1555;
    //    stRgnAttr.unAttr.stOverlayEx.u32BgColor = 0;
    //    stRgnAttr.unAttr.stOverlayEx.u32CanvasNum = 2;
    //    stRgnAttr.enType = OVERLAYEX_RGN;
    //    stChnAttr.bShow = (Chip_BOOL) e.show;
    //    stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32X = e.x;
    //    stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32Y = e.y;
    //    stChnAttr.unChnAttr.stOverlayExChn.u32Layer = e.layer;
    //    stChnAttr.enType = OVERLAYEX_RGN;
    //
    //    s32Ret = ChipRgnCreate(id, &stRgnAttr);
    //    if (s32Ret != 0) {
    //        emxloge("chn %d ChipRgnCreate %d failed! s32Ret: 0x%x.\n", m_chn, id, s32Ret);
    //        return -1;
    //    }
    //    s32Ret = ChipRgnAttachToChn(handle, &stChn, &stChnAttr);
    //    if (s32Ret != 0) {
    //        emxloge("chn %d ChipRgnAttachToChn %d failed! s32Ret: 0x%x.\n", m_chn, id, s32Ret);
    //        ChipRgnDestroy(id);
    //    }
    // }
}

void DefaultOsd::RegionDestroyAll(const std::list<RegionInfo> &region) {
    //! 销毁所有region
    // for(auto &e : myRegionList){
    //    ChipRgnDestroy(region);
    // }
}

void DefaultOsd::SetBitMap(int region, uint8_t *pix, int w, int h) {
    //! 这里需要把一张文字bitmap图片叠加到region中
    //    BITMAP_S stBitmap = {};
    //    stBitmap.u32Width = w;
    //    stBitmap.u32Height = h;
    //    stBitmap.pData = pix;
    //    stBitmap.enPixelFormat = PIXEL_FORMAT_ARGB_1555;
    //    ChipRgnSetBitMap(region, &stBitmap);
}

ErrCodeE DefaultOsd::ImageToCanvas(int region, const std::string &data) {
    //! 这里需要将图片更新到画布
    //    int s32Ret;
    //    RGN_CANVAS_INFO_S stCanvasInfo = {};
    //    s32Ret = ChipRgnGetCanvasInfo(region, &stCanvasInfo);
    //    if (s32Ret != 0) {
    //        emxloge("chn %d ChipRgnGetCanvasInfo %d failed! s32Ret: 0x%x.\n", m_chn, region, s32Ret);
    //        return ErrCodeE::Failure;
    //    }
    //
    //    if (!stCanvasInfo.pu8VirtAddr) {
    //        emxloge("chn %d handle %d pu8VirtAddr null\n", m_chn, region);
    //        return ErrCodeE::Failure;
    //    }
    //
    //    auto dst = (char *) stCanvasInfo.pu8VirtAddr;
    //    auto src = (const char *) data.data();
    //
    //    for (int i = 0; i < (int) stCanvasInfo.stSize.u32Height; i++) {
    //        memcpy(dst, src, stCanvasInfo.stSize.u32Width * 4);
    //        dst += stCanvasInfo.u32Stride;
    //        src += stCanvasInfo.stSize.u32Width * 4;
    //    }
    //
    //    s32Ret = ChipRgnUpdateCanvas(region);
    //    if (s32Ret != 0) {
    //        emxloge("chn %d ChipRgnUpdateCanvas %d failed! s32Ret: 0x%x.\n", m_chn, region, s32Ret);
    //        return ErrCodeE::Failure;
    //    }
    return ErrCodeE::Success;
}