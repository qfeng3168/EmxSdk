/**
 * @author tfh
 * @brief
 * @date 2024-09-12
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef EMX_DefaultAov_HPP
#define EMX_DefaultAov_HPP

#include "media/server/MediaServerAov.hpp"
#include "EmxCore.hpp"

namespace Emx {

    class DefaultAov : public MediaServerAovChn {
    public:
        DefaultAov(int32_t chn): MediaServerAovChn(chn){}

        virtual ~DefaultAov() {};

        ErrCodeE OnCreate() override;

        void OnDestroy() override;

        ErrCodeE OnParamChanged() override;
    };
}// namespace Emx

#endif// EMX_DefaultAov_HPP
