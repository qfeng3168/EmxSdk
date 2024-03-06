//
// Created by xiong on 2022/4/27.
//

#ifndef EMX_DefaultAi_HPP
#define EMX_DefaultAi_HPP

#include "media/server/MediaServerAi.hpp"
#include "EmxCore.hpp"

namespace Emx {


    class DefaultAi : public MediaServerAiChn {
    public:
        DefaultAi(int32_t chn) : MediaServerAiChn(chn) {}

        virtual ~DefaultAi() {};
    protected:

        virtual MediaAiPlugin *MakePlugin(const char *name) override;

    protected:
    };
}

#endif //EMX_DefaultAi_HPP
