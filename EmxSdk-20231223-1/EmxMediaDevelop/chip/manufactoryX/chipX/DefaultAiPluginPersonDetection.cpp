//
// Created by xiong on 2023/9/13.
//

#include "DefaultAiPluginPersonDetection.hpp"
#include "media/server/MediaServerCtx.hpp"
#include "EmxCore.hpp"

using namespace Emx;

void DefaultAiPluginPersonDetection::Create(Json::Value &param) {
    //! AI的m_param参数需要自己负责更新
    m_param = param;
    if (!m_param["ena"].asBool()) {
        return;
    }
    //! 加载需要的参数
    Json::Value root;
    MediaServerCtx::GetInst()->GetCfg(m_name.c_str(), root);
    auto &json = root[m_chn];
    m_vpssGrp = json["vpssGrp"].asInt();
    m_vpssChn = json["vpssChn"].asInt();
    m_threshold = json["threshold"].asInt();
    //! 开始Ai处理
    Start(m_param);
}

void DefaultAiPluginPersonDetection::Destroy() {
    //! 结束Ai处理并销毁
    if (!m_param["ena"].asBool())
        return;
    Stop(m_param);
}

ErrCodeE DefaultAiPluginPersonDetection::ParamChange(Json::Value &paramNew) {
    if (paramNew["ena"].asBool() != m_param["ena"].asBool()) {
        if (paramNew["ena"].asBool()) {
            Start(paramNew);
        } else {
            Stop(m_param);
        }
        m_param = paramNew;
    }
    return ErrCodeE::Success;
}


void DefaultAiPluginPersonDetection::Start(Json::Value &param) {
    //! 初始化模型等操作
    //! ...
    //! 使用job获取线程池资源，其中OnGetFrame运行在线程池中，OnGetFrameComplete运行在EmxMediaServer的主loop中
    m_got = false;
    m_job.Create(std::bind(&DefaultAiPluginPersonDetection::OnGetFrame, this, ph_1),
                 std::bind(&DefaultAiPluginPersonDetection::OnGetFrameComplete, this, ph_1));
    emxlogi("done\n");
}

void DefaultAiPluginPersonDetection::Stop(Json::Value &param) {
    m_job.Destroy();
    emxlogi("done\n");
}

void DefaultAiPluginPersonDetection::OnGetFrame(void *arg) {
//    int s32Ret = 0;
//    s32Ret = ChipVpssGetChnFrame(m_vpssGrp, m_vpssChn, &stFrame, 500);
//    if (s32Ret != 0) {
//        emxloge("chn %d ChipVpssGetChnFrame failed with %#x\n", m_chn, s32Ret);
//        return;
//    }
//    ChipAiMobileDetV2_Pedestrian_D0(DefaultAiHandle::GetInst()->aiHandle, &stFrame, &obj);
//    if (obj.size == 0)
//        return;
    //! 这里需要填充m_data
//    auto &header = m_data.header;
//    header.chn = m_chn;
//    header.type = MediaInfAiData::TypeE::PersonDetection;
//    header.timestampMs = Time::GetMs64();
//    header.srcPicSize.w = (int) stFrame.stVFrame.u32Width;
//    header.srcPicSize.h = (int) stFrame.stVFrame.u32Height;
//    m_data.num = (int) obj.size;
//    m_data.Alloc(m_data.num);
//    for (int i = 0; i < (int) obj.size; i++) {
//        auto &bbox = obj.info[i].bbox;
//        m_data.array[i].Set((int) bbox.x1, (int) bbox.y1, (int) (bbox.x2 - bbox.x1), (int) (bbox.y2 - bbox.y1));
//    }
//    ChipAiFree(&obj);
//    s32Ret = ChipVpssReleaseChnFrame(m_vpssGrp, m_vpssChn, &stFrame);
//    if (s32Ret != 0) {
//        emxloge("chn %d ChipVpssReleaseChnFrame failed with %#x\n", m_chn, s32Ret);
//    }
    m_got = true;
}

void DefaultAiPluginPersonDetection::OnGetFrameComplete(void *arg) {
    //! 得到数据后这里才能push出去
    if (!m_got)
        return;
    MediaAiJob::PushAiData(&m_data);
    m_data.Free();
    m_got = false;
}
