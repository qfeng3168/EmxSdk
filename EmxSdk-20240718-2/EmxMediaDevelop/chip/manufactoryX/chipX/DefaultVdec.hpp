/**
 * @author tfh
 * @brief
 * @date 2024-01-06
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef EMX_DefaultVdec_HPP
#define EMX_DefaultVdec_HPP

#include "media/server/MediaServerVdec.hpp"
#include "EmxCore.hpp"

namespace Emx {

    class DefaultVdec : public MediaServerVdecChn {
    public:
        DefaultVdec(int32_t chn)
                : MediaServerVdecChn(chn){
        }

        virtual ~DefaultVdec() {};

        ErrCodeE OnCreate() override;

        void OnDestroy() override;

        ErrCodeE OnParamChanged() override;

    protected:
        ErrCodeE SendFrame(Emx::MediaServerVdecChn::FrameData &frame) override;

    };
}// namespace Emx

#endif// EMX_DefaultVdec_HPP
