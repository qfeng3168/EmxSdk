//
// Created by xiong on 2020/10/30.
//

#ifndef EMX_MediaFactory_HPP
#define EMX_MediaFactory_HPP

#include "media/common/MediaTypedef.hpp"

namespace Emx {
    class MediaServer;

    class MediaServerModuleChnBase;

    class MediaFactory {
    public:
        static MediaFactory *GetMediaFactory();

        static void PrintMediaServerInfo();

        virtual ErrCodeE SwitchScene(const char *curScene, const char *newScene);

        virtual MediaServer *MakeServer();

        MediaServerModuleChnBase *MakeModuleChn(MediaModE mod, int32_t chn) {
            switch (mod) {
                case MediaModE::System:
                    return MakeSystemChn(chn);
                case MediaModE::Isp:
                    return MakeIspChn(chn);
                case MediaModE::Venc:
                    return MakeVencChn(chn);
                case MediaModE::Aenc:
                    return MakeAencChn(chn);
                case MediaModE::Adec:
                    return MakeAdecChn(chn);
                case MediaModE::Snap:
                    return MakeSnapChn(chn);
                case MediaModE::QrScan:
                    return MakeQrScanChn(chn);
                case MediaModE::Osd:
                    return MakeOsdChn(chn);
                case MediaModE::Ai:
                    return MakeAiChn(chn);
                case MediaModE::Vi:
                    return MakeViChn(chn);
                case MediaModE::Vdec:
                    return MakeVdecChn(chn);
                case MediaModE::Vo:
                    return MakeVoChn(chn);
                case MediaModE::Draw:
                    return MakeDrawChn(chn);
                default:
                    return nullptr;
            }
        }

        virtual MediaServerModuleChnBase *MakeSystemChn(int32_t chn) = 0;

        virtual MediaServerModuleChnBase *MakeIspChn(int32_t chn) = 0;

        virtual MediaServerModuleChnBase *MakeVencChn(int32_t chn) = 0;

        virtual MediaServerModuleChnBase *MakeAencChn(int32_t chn) = 0;

        virtual MediaServerModuleChnBase *MakeAdecChn(int32_t chn) = 0;

        virtual MediaServerModuleChnBase *MakeSnapChn(int32_t chn) = 0;

        virtual MediaServerModuleChnBase *MakeQrScanChn(int32_t chn) = 0;

        virtual MediaServerModuleChnBase *MakeOsdChn(int32_t chn) = 0;

        virtual MediaServerModuleChnBase *MakeAiChn(int32_t chn) = 0;

        virtual MediaServerModuleChnBase *MakeViChn(int32_t chn) = 0;

        virtual MediaServerModuleChnBase *MakeVdecChn(int32_t chn) = 0;

        virtual MediaServerModuleChnBase *MakeVoChn(int32_t chn) = 0;
       
        virtual MediaServerModuleChnBase *MakeDrawChn(int32_t chn) = 0;
    };
}


#endif //EMX_MediaFactory_HPP
