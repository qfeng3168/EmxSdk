//
// Created by tfh on 2024/01/06.
//
#include "DefaultVo.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

ErrCodeE DefaultVo::OnCreate() {
    return ErrCodeE::Success;
}

void DefaultVo::OnDestroy() {

}

ErrCodeE DefaultVo::OnParamChanged(){
    return ErrCodeE::Success;
}

ErrCodeE DefaultVo::runOneViSendVo() {
    return ErrCodeE::Success;
}


