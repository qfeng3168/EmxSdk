//
// Created by xiong on 2021/7/13.
//
#include "media/server/MediaFactory.hpp"
#include "chipXDefaultFactory.hpp"
#include "DefaultAdec.hpp"
#include "DefaultAenc.hpp"
#include "DefaultDraw.hpp"
#include "DefaultIsp.hpp"
#include "DefaultOsd.hpp"
#include "DefaultQrScan.hpp"
#include "DefaultSnap.hpp"
#include "DefaultSystem.hpp"
#include "DefaultVdec.hpp"
#include "DefaultVenc.hpp"
#include "DefaultVi.hpp"
#include "DefaultVo.hpp"
#include "DefaultAov.hpp"


using namespace Emx;

MediaServerModuleChnBase *chipXDefaultFactory::MakeSystemChn(int32_t chn) { return new DefaultSystem(chn); }

MediaServerModuleChnBase *chipXDefaultFactory::MakeIspChn(int32_t chn) { return new DefaultIsp(chn); }

MediaServerModuleChnBase *chipXDefaultFactory::MakeVencChn(int32_t chn) { return new DefaultVenc(chn); }

MediaServerModuleChnBase *chipXDefaultFactory::MakeAencChn(int32_t chn) { return new DefaultAenc(chn); }

MediaServerModuleChnBase *chipXDefaultFactory::MakeAdecChn(int32_t chn) { return new DefaultAdec(chn); }

MediaServerModuleChnBase *chipXDefaultFactory::MakeSnapChn(int32_t chn) { return new DefaultSnap(chn); }

MediaServerModuleChnBase *chipXDefaultFactory::MakeQrScanChn(int32_t chn) { return new DefaultQrScan(chn); }

MediaServerModuleChnBase *chipXDefaultFactory::MakeOsdChn(int32_t chn) { return new DefaultOsd(chn); }

// MediaServerModuleChnBase *chipXDefaultFactory::MakeAiChn(int32_t chn) { return new DefaultAi(chn); }

MediaServerModuleChnBase *chipXDefaultFactory::MakeViChn(int32_t chn) { return new DefaultVi(chn); }

MediaServerModuleChnBase *chipXDefaultFactory::MakeVdecChn(int32_t chn) { return new DefaultVdec(chn); }

MediaServerModuleChnBase *chipXDefaultFactory::MakeVoChn(int32_t chn) { return new DefaultVo(chn); }

MediaServerModuleChnBase *chipXDefaultFactory::MakeDrawChn(int32_t chn) { return new DefaultDraw(chn); }

MediaServerModuleChnBase *chipXDefaultFactory::MakeAovChn(int32_t chn) { return new DefaultAov(chn); }
