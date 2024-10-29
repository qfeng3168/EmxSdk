//
// Created by tfh on 2024/09/12.
//
#include "DefaultAov.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

ErrCodeE DefaultAov::OnCreate() {
    return ErrCodeE::Success;
}

void DefaultAov::OnDestroy() {

}

ErrCodeE DefaultAov::OnParamChanged(){
    return ErrCodeE::Success;
}


