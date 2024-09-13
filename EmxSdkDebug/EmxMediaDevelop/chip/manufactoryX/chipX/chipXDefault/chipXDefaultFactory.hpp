//
// Created by xiong on 2021/7/13.
//

#ifndef EMX_chipXDefaultFactory_HPP
#define EMX_chipXDefaultFactory_HPP

#include "media/server/MediaFactory.hpp"

namespace Emx {
    class chipXDefaultFactory : public MediaFactory {
    public:
        MediaServerModuleChnBase *MakeSystemChn(int32_t chn) override;

        MediaServerModuleChnBase *MakeVencChn(int32_t chn) override;

        MediaServerModuleChnBase *MakeAencChn(int32_t chn) override;

        MediaServerModuleChnBase *MakeAdecChn(int32_t chn) override;

        MediaServerModuleChnBase *MakeIspChn(int32_t chn) override;

        MediaServerModuleChnBase *MakeSnapChn(int32_t chn) override;

        MediaServerModuleChnBase *MakeQrScanChn(int32_t chn) override;

        MediaServerModuleChnBase *MakeOsdChn(int32_t chn) override;

        // MediaServerModuleChnBase *MakeAiChn(int32_t chn) override;

        MediaServerModuleChnBase *MakeViChn(int32_t chn) override;

        MediaServerModuleChnBase *MakeVdecChn(int32_t chn) override;

        MediaServerModuleChnBase *MakeVoChn(int32_t chn) override;

        MediaServerModuleChnBase *MakeDrawChn(int32_t chn) override;

        MediaServerModuleChnBase *MakeAovChn(int32_t chn) override;
    };
}// namespace Emx
#endif//EMX_chipXDefaultFactory_HPP
