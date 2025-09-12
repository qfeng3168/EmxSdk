//
// Created by tfh on 2024/01/06.
//
#include "DefaultVdec.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

ErrCodeE DefaultVdec::OnCreate() {
    return ErrCodeE::Success;
}
ErrCodeE DefaultVdec::OnParamChanged(){
    return ErrCodeE::Success;
}
void DefaultVdec::OnDestroy() {

}

ErrCodeE DefaultVdec::SendFrame(FrameData &frame) {
    return ErrCodeE::Success;
}

ErrCodeE DefaultVdec::SendFrameDecode(FrameData &data, FrameData &raw) {
    return ErrCodeE::Success;
}
