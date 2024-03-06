//
// Created by xiong on 2023/9/4.
//
#include "EmxCore.hpp"
#include "EmxVersion.h"
#include MEDIA_PROFILE_FACTORY_HEADER

using namespace Emx;

MediaFactory *MediaFactory::GetMediaFactory() {
    static MEDIA_PROFILE_FACTORY_CLASS factory;
    return &factory;
}

void MediaFactory::PrintMediaServerInfo() {
    emxlogi("EmxMediaVersion=%s\n", EmxMediaVersion);
    emxlogi("EmxSdkVersion=%s\n", EMX_SDK_VERSION);
    emxlogi("ChipManufactory=%s\n", ChipManufactory);
    emxlogi("ChipSerial=%s\n", ChipSerial);
    emxlogi("ChipSdkVersion=%s\n", ChipSdkVersion);
    emxlogi("Profile=%s\n", Profile);
}
