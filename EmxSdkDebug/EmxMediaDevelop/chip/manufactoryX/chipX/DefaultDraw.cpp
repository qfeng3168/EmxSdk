//
// Created by xiong on 2021/8/10.
//
#include "DefaultDraw.hpp"
#include "media/server/MediaModulesAll.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

ErrCodeE DefaultDraw::OnCreate() {
     //! 加载需要的参数

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
    return ErrCodeE::Success;
}

void DefaultDraw::OnDestroy() {
    //! 销毁操作
}

Size DefaultDraw::GetChnPicSize() { 
     //! Initialize中提供的宽高信息
    return {m_chnWidth, m_chnHeight};
}

void DefaultDraw::DrawSetItem(const int id, ParamItem &item) {
   //! 根据id设置对应画线框属性
    //  设置item
}

void DefaultDraw::DrawCreateAll(std::list<DrawInfo> &info){
     //! 创建所有画线框，厂商按照自己的特性来，需要赋值使用的e.id
    //! 需要自己保证id的唯一性即可
    // for(auto &e: info){
    //     e.id = Draw(e.item);
    // }
    
}

void DefaultDraw::DrawDestroyAll(std::list<DrawInfo> &info) {
    // for(auto &e: info){
    // 根据id销毁对应画线框
    // 销毁data 
    //      DeleteDraw(e.id);
    //      e.id = -1;
    // }
}
