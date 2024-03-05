//
// Created by xiong on 2022/4/27.
//
#include "DefaultAi.hpp"
#include "media/server/MediaServerCtx.hpp"
#include "DefaultAiPluginMotionDetection.hpp"
#include "DefaultAiPluginPersonDetection.hpp"

using namespace Emx;

MediaAiPlugin *DefaultAi::MakePlugin(const char *name) {
    if (strcmp(name, "MotionDetection") == 0)
        return new DefaultAiPluginMotionDetection(m_chn);
    if (strcmp(name, "PersonDetection") == 0)
        return new DefaultAiPluginPersonDetection(m_chn);
    emxloge("plugin %s not supported\n", name);
    return new MediaAiPlugin(m_chn, name);
}
