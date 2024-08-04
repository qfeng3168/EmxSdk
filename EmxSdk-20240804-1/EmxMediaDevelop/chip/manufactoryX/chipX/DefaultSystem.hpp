//
// Created by xiong on 2020/10/27.
//

#ifndef EMX_DefaultSystem_HPP
#define EMX_DefaultSystem_HPP

#include "media/server/MediaServerSystem.hpp"
#include "EmxCore.hpp"

namespace Emx {

    class DefaultSystem : public MediaServerSystemChn {
    public:
        DefaultSystem(int32_t chn) : MediaServerSystemChn(chn) {}

        virtual ~DefaultSystem() {}

        ErrCodeE OnCreate() override;

        void OnDestroy() override;
    };
}


#endif //EMX_DefaultSystem_HPP
