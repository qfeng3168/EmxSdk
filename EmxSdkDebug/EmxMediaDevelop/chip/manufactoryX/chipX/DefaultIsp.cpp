//
// Created by xiong on 2023/9/7.
//

#include "DefaultIsp.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

ErrCodeE DefaultIsp::OnCreate() {
    //! 加载需要的参数
//    Json::Value root;
//    MediaServerCtx::GetInst()->GetCfg(m_module->m_name.c_str(), root);
//    auto &json = root[m_chn];
//    m_pipe = json["pipe"].asInt();
    //! 初始化运行模式为白天
    m_pParam = &m_param;
    SetRunMode(RunModeE::Day);
    return ErrCodeE::Success;
}

void DefaultIsp::OnDestroy() {
    //! 销毁操作
}

ErrCodeE DefaultIsp::OnParamChanged() {
    //! 当参数发生变化时需要进行配置
    //! m_param是当前参数，m_paramNew是新到来的参数
    m_pParam = &m_paramNew;
    //设置参数
    m_pParam = &m_param;
    return ErrCodeE::Success;
}

ErrCodeE DefaultIsp::SetRunMode(RunModeE mode) {
    if (m_mode == mode) {
        return ErrCodeE::Success;
    }
    //! 这里进行运行模式的切换，可以是更换PQ文件或者其他类似操作
//    Json::Value root;
//    MediaServerCtx::GetInst()->GetCfg(m_module->m_name.c_str(), root);
//    auto &json = root[m_chn];
//    const char *name;
//    int fps;
//    switch (mode) {
//        case RunModeE::Day:
//            name = "day";
//            fps = json["fps"]["day"].asInt();
//            break;
//        case RunModeE::Night:
//            name = "night";
//            fps = json["fps"]["night"].asInt();
//            break;
//        case RunModeE::NightWithLight:
//            name = "nightWithLight";
//            fps = json["fps"]["night"].asInt();
//            break;
//        case RunModeE::User:
//            name = "user";
//            fps = json["fps"]["day"].asInt();
//            break;
//        default:
//            emxloge("unknown mode %d\n", (int) mode);
//            return ErrCodeE::IllegalParam;
//    }
//
//    char path[EMX_MAX_PATH_SIZE];
//    int s32Ret = 0;
//    Chip_BIN_SetBinName(m_wdrMode, SysEnv::GetMediaCfgFilePath(path, "isp/%s", json["pq"][name].asCString()));
//    Chip_CHAR binName[BIN_FILE_LENGTH];
//    s32Ret = Chip_BIN_GetBinName(binName);
//    if (s32Ret != 0) {
//        emxloge("GetBinName(%s) fail\n", binName);
//        return ErrCodeE::Failure;
//    }
//    std::string buffer;
//    if (File::Read(path, buffer) != ErrCodeE::Success) {
//        emxloge("read %s failed\n", path);
//        return ErrCodeE::Failure;
//    }
//    s32Ret = Chip_BIN_ImportBinData((uint8_t *) buffer.data(), (uint32_t) buffer.size());
//    if (s32Ret != 0) {
//        emxloge("PQBin file(%s), buffer size(%d)\n", path, buffer.size());
//        emxloge("Chip_BIN_ImportBinData failed with %#x!\n", s32Ret);
//    }
//
//    ISP_PUB_ATTR_S pubAttr = {};
//    if (ChipIspGetPubAttr(m_pipe, &pubAttr) == 0) {
//        if (pubAttr.f32FrameRate != (float) fps) {
//            pubAttr.f32FrameRate = (float) fps;
//            if (ChipIspSetPubAttr(m_pipe, &pubAttr) == 0) {
//                SetAntiFlicker(m_param.antiFlickerHz, m_param.forceAntiFlickerEna);
//                //todo:vpss and venc change fps src
//            } else {
//                emxloge("ChipIspSetPubAttr %d failed with %#x!\n", m_pipe, s32Ret);
//            }
//        }
//    } else {
//        emxloge("ChipIspGetPubAttr %d failed with %#x!\n", m_pipe, s32Ret);
//    }
    m_mode = mode;
    return ErrCodeE::Success;
}

ErrCodeE DefaultIsp::Collection(Info &info) {
    //! 这里会被周期调用执行，用来获取isp信息
//    ISP_EXP_INFO_S stExpInfo = {};
//    if (ChipIspQueryExposureInfo(m_pipe, &stExpInfo) != 0)
//        return ErrCodeE::Failure;
//    int redGain = 0;
//    int blueGain = 0;
//    if (m_mode == RunModeE::Night) {
//        if (ChipIspGetGrayWorldAwbInfo(m_pipe, &redGain, &blueGain) != 0)
//            return ErrCodeE::Failure;
//    } else {
//        ISP_WB_INFO_S stWBInfo = {};
//        if (ChipIspQueryWBInfo(m_pipe, &stWBInfo) != 0)
//            return ErrCodeE::Failure;
//        redGain = stWBInfo.u16Rgain;
//        blueGain = stWBInfo.u16Bgain;
//    }
    //! 填充info
//    info.chn = m_chn;
//    info.iso = (int) stExpInfo.u32ISO;
//    info.redGain = redGain;
//    info.blueGain = blueGain;
//    info.luma = (int) stExpInfo.u8AveLum;
    return ErrCodeE::Success;
}
