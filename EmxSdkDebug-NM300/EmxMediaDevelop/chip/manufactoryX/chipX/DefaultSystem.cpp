//
// Created by xiong on 2020/10/27.
//
#include "DefaultSystem.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

ErrCodeE DefaultSystem::OnCreate() {
    //! 加载需要的参数
//    Json::Value root;
//    MediaServerCtx::GetInst()->GetCfg(m_module->m_name.c_str(), root);
//    auto &json = root[m_chn];
    //! 初始化整个系统，例如vb/vi/vpss
//    ErrCodeE e;
//    e = InitVb(json["vb"]);
//    if (e != ErrCodeE::Success) {
//        emxlogc("Init vb failed : %d\n", e);
//        return e;
//    }
//    e = InitVi(json["vi"]);
//    if (e != ErrCodeE::Success) {
//        DeInitVb(json["vb"]);
//        emxlogc("InitSensor failed : %d\n", e);
//        return e;
//    }
//    e = InitVpss(json["vpss"]);
//    if (e != ErrCodeE::Success) {
//        DeInitVi(json["vi"]);
//        DeInitVb(json["vb"]);
//        emxlogc("InitVpss failed : %d\n", e);
//        return e;
//    }
    emxlogi("Start system done\n");
    return ErrCodeE::Success;
}

void DefaultSystem::OnDestroy() {
    //! 加载需要的参数
//    Json::Value root;
//    MediaServerCtx::GetInst()->GetCfg(m_module->m_name.c_str(), root);
//    auto &json = root[m_chn];
    //! 销毁整个系统，例如vb/vi/vpss
//    DeInitVpss(json["vpss"]);
//    DeInitVi(json["vi"]);
//    DeInitVb(json["vb"]);
    emxlogi("Stop system done\n");
}
