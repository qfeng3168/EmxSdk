//
// Created by xiong on 2023/9/11.
//

#include "MotionDetection.hpp"
#include "EmxCore.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

void MotionDetection::Create(const char *workspace, Json::Value &param) {
    if (m_created) return;
    m_param = param[m_name];
    //! 加载本算法需要的芯片参数
    Json::Value root;
    char path[EMX_MAX_PATH_SIZE] = {};
    snprintf(path, sizeof(path), "%s/AiPlugin/configs/%s.json", SysEnv::GetDataDir(), m_name.c_str());
    auto e = EasyJson::Load(path, root);
    if (e != ErrCodeE::Success) {
        emxloge("get media %s configuration failed\n", path);
        return;
    }
    m_pause = false;
    auto &json = root[m_chn];
    m_vpssGrp = json["vpssGrp"].asInt();
    m_vpssChn = json["vpssChn"].asInt();
    m_interval = json["interval"].asInt();
    m_threshold = json["threshold"].asInt();
    m_minArea = json["minArea"].asInt();
    m_ena = m_param["ena"].asBool();
    //! 开始Ai处理
    Start(m_param);
    m_created = true;
    emxlogi("create done\n");
}

void MotionDetection::Destroy() {
    //! 结束Ai处理并销毁
    if (m_created) {
        Stop(m_param);
        m_created = false;
    }
}

ErrCodeE MotionDetection::SetParam(const Json::Value &paramNew) {
    m_param = paramNew[m_name];
    //刷新参数或者设置启停
    bool enable = m_param["ena"].asBool();
    if (m_param.isMember("threshold")) m_threshold = m_param["threshold"].asInt();
    if (enable != m_ena) {
        if (m_ena) {
            m_job.Destroy();
        } else {
            m_count = m_interval - 1;
            m_job.Create(std::bind(&MotionDetection::OnGetFrame, this, ph_1),
                    std::bind(&MotionDetection::OnGetFrameComplete, this, ph_1));
        }
        m_ena = enable;
    }
    return ErrCodeE::Success;
}

void MotionDetection::Start(const Json::Value &param) {
    if (m_started) return;
    if (!param["ena"].asBool()) return;
    //! 初始化模型等操作
    //! ...
    //! 使用job获取线程池资源，其中OnGetFrame运行在线程池中，OnGetFrameComplete运行在EmxMediaServer的主loop中
    m_got = false;
    if (m_ena) {
        m_count = m_interval - 1;
        m_job.Create(std::bind(&MotionDetection::OnGetFrame, this, ph_1),
                std::bind(&MotionDetection::OnGetFrameComplete, this, ph_1));
    }
    m_started = true;
    emxlogi("start done\n");
}

void MotionDetection::Stop(const Json::Value &param) {
    //! 结束任务
    if (m_ena) { m_job.Destroy(); }
    if (m_started) {
        m_started = false;
        emxlogi("done\n");
    }
}


void MotionDetection::OnGetFrame(void *arg) {
    if (m_pause) return;
    //! 这个函数会被循环调用，运行在线程池中，允许阻塞操作
    //    int s32Ret = 0;
    //    s32Ret = ChipVpssGetChnFrame(m_vpssGrp, m_vpssChn, &stFrame, 500);
    //    if (s32Ret != 0) {
    //        emxloge("chn %d ChipVpssGetChnFrame failed with %#x\n", m_chn, s32Ret);
    //        return;
    //    }

    //m_count 背景刷新间隔
    //m_count++;
    // if (m_count == m_interval) {
    //   m_count = 0;
    //    s32Ret =ChipAi_MotionDetection_Background(m_aiHandle, &stFrame);
    //    if (s32Ret != 0) { emxloge("chn %d update background failed with %#x\n", m_chn, s32Ret); }
    // }

    //    ChipAiMotionDetection(&stFrame, &obj, m_threshold, m_minArea);
    //    if (obj.size == 0)
    //        return;
    //! 这里需要填充m_data
    //    auto &header = m_data.header;
    //    header.chn = m_chn;
    //    header.type = MediaInfAiData::TypeE::MotionDetection;
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

void MotionDetection::OnGetFrameComplete(void *arg) {
    //! 得到数据后只能在Complete push出去
    if (!m_got) return;
    MediaAiJob::PushAiData(&m_data);
    m_data.Free();
    m_got = false;
}

ErrCodeE MotionDetection::Ctrl(const Json::Value &in, Json::Value &out) {
    ErrCodeE e = ErrCodeE::OperationNotSupport;
    if (in.isMember("motionDetection")) {
        m_pause = in["motionDetection"]["pause"].asBool();
        m_count = m_interval - 1;
        e = ErrCodeE::Success;
    }
    return e;
}