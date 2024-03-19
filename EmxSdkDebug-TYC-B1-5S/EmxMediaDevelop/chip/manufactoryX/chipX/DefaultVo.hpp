/**
 * @author tfh
 * @brief
 * @date 2024-01-06
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef EMX_DefaultVo_HPP
#define EMX_DefaultVo_HPP

#include "media/server/MediaServerVo.hpp"
#include "EmxCore.hpp"

namespace Emx {

    class DefaultVo : public MediaServerVoChn {
    public:
        DefaultVo(int32_t chn)
                : MediaServerVoChn(chn){
        }

        virtual ~DefaultVo() {};

        ErrCodeE OnCreate() override;

        void OnDestroy() override;

        ErrCodeE OnParamChanged() override;
    protected:
        ErrCodeE runOneViSendVo() override;

    };
}// namespace Emx

#endif// EMX_DefaultVo_HPP
