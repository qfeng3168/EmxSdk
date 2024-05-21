//
// Created by tfh on 2024/01/06.
//

#ifndef EMX_MediaVdec_HPP
#define EMX_MediaVdec_HPP

#include "media/common/MediaInfVdec.hpp"
#include "MediaParamCBase.hpp"
#include "media/common/MediaRingBuffer.hpp"

namespace Emx {
    //!@brief 视频编码
    class MediaVdec : public MediaInfVdec, public MediaParamCBase<MediaInfVdec::Param> {
    public:
        explicit MediaVdec(int32_t chn) : MediaParamCBase(MediaModE::Vdec, chn) {}
    };

    class MediaVdecStream {
    public:
        explicit MediaVdecStream(int chn) : m_chn(chn) {}

        ErrCodeE Create();

        ErrCodeE Destroy();

        ErrCodeE PushFrame(MediaInfVdec::FrameData &frame, int32_t timeoutMs);

    private:

        int m_chn;
        int m_key;
        MediaRingBufferWriter m_ring;
    };
}

#endif //EMX_MediaVdec_HPP
