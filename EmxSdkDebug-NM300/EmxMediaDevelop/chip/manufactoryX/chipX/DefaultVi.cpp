//
// Created by xiong on 2023/9/6.
//

#include "DefaultVi.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

ErrCodeE DefaultVi::OnCreate() {
    //! 加载需要的参数
    Json::Value root;
    MediaServerCtx::GetInst()->GetCfg(m_module->m_name.c_str(), root);
    auto &json = root[m_chn]["attr"];
    m_pipe = json["ViPipe"].asInt();
    m_viChn = json["ViChn"].asInt();
    m_originFlip = json["flip"].asBool();
    m_originMirror = json["mirror"].asBool();

    //! 必要的初始化创建操作
    //! ...

    //! 初始化翻转状态
    SetMirrorFlip(m_param);
    emxlogi("chn %d done\n", m_chn);
    return ErrCodeE::Success;
}

void DefaultVi::OnDestroy() {
    //! 必要的销毁操作
    //! ...
    emxlogd("chn %d done\n", m_chn);
}

ErrCodeE DefaultVi::OnParamChanged() {
    return SetMirrorFlip(m_paramNew);
}

ErrCodeE DefaultVi::SetMirrorFlip(MediaInfVi::Param &param) {
    //! 这里需要根据原始出厂的翻转状态，使用异或操作确定最终配置参数
//    int ret = ChipSetChnFlipMirror(m_pipe, m_viChn, param.flip != m_originFlip,
//                                   param.mirror != m_originMirror);
//    if (ret != 0) {
//        emxloge("chn %d ChipSetChnFlipMirror failed with %#x!\n", m_chn, ret);
//        return ErrCodeE::Failure;
//    }
    return ErrCodeE::Success;
}

ErrCodeE DefaultVi::SetHideStatus(bool enable){
    if(enable){
        //将vi数据流全部置黑
    }else{
        //vi数据正常输出sorsor送入画面
    }
    return ErrCodeE::Success;
}
