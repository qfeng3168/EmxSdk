//
// Created by tangfahong on 2024/4/1.
//

#include "PassengerFlowStatistics.hpp"
#include "EmxCore.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

void PassengerFlowStatistics::Create(const char *workspace, Json::Value &param) {
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
    auto &json = root[m_chn];
    m_vbId = json["vbId"].asInt();
    m_vpssGrp = json["vpssGrp"].asInt();
    m_vpssChn = json["vpssChn"].asInt();
    m_threshold = json["threshold"].asFloat();
    m_modePath = SysEnv::GetDataDir();
    m_modePath += "/AiPlugin/";
    m_modePath += json["modelPath"].asCString();
    m_ena = false;
    emxlogd("m_modePath = %s\n", m_modePath.c_str());

    //! 开始Ai处理
    Start(m_param);
    m_created = true;
    SetParam(m_param);
}

void PassengerFlowStatistics::Destroy() {
    if (m_created) {
        Stop(m_param);
        m_created = false;
    }
}

ErrCodeE PassengerFlowStatistics::ParamSet() {
    int lineX1 = m_param["lineX1"].asInt() * m_width / 10000;
    int lineY1 = m_param["lineY1"].asInt() * m_height / 10000;
    int lineX2 = m_param["lineX2"].asInt() * m_width / 10000;
    int lineY2 = m_param["lineY2"].asInt() * m_height / 10000;

    if (lineX1 > (int) m_width) {
        lineX1 = (int) m_width;
    } else if (lineX1 < 0) {
        lineX1 = 0;
    }
    if (lineX2 > (int) m_width) {
        lineX2 = (int) m_width;
    } else if (lineX2 < 0) {
        lineX2 = 0;
    }
    //获取客流统计越线方向
    int statisticsMode = m_param["direction"].asInt() ? 1 : 0;
    emxlogi("ConsumerCounting_Line(%d,%d)->(%d,%d)!statisticsMode=%d\n", lineX1, lineY1, lineX2, lineY2,
            statisticsMode);
    // int ret = AI_APP_ConsumerCounting_Line(m_appHandle, lineX1, lineY1, lineX2, lineY2, statisticsMode);
    // if (ret != 0) {
    //     emxloge("AI_APP_ConsumerCounting_Line err ,ret=%d!\n", ret);
    //     return ErrCodeE::IllegalParam;
    // }
    return ErrCodeE::Success;
}

ErrCodeE PassengerFlowStatistics::SetParam(const Json::Value &paramNew) {
    m_param = paramNew[m_name];
    bool enable = m_param["ena"].asBool();
    if (enable != m_ena) {
        if (m_ena) {
            m_job.Destroy();
        } else {
            m_job.Create(std::bind(&PassengerFlowStatistics::OnGetFrame, this, ph_1),
                    std::bind(&PassengerFlowStatistics::OnGetFrameComplete, this, ph_1), nullptr, "PassengerFlow");
        }
        m_ena = enable;
    }

    ParamSet();
    return ErrCodeE::Success;
}

void PassengerFlowStatistics::Start(const Json::Value &param) {
    // int ret = 0;
    //初始化算法，加载模型
    // ret = AI_CreateHandle(&m_aiHandle);
    // if (ret != 0) {
    //     emxloge("handle create failed with %#x!\n", ret);
    //     return;
    // }
    // ret = AI_APP_CreateHandle(&m_appHandle, m_aiHandle);
    // if (ret != 0) {
    //     emxloge("AI_APP_CreateHandle failed with %#x\n", ret);
    //     return;
    // }
    // ret = AI_APP_PersonCapture_Init(m_appHandle, bufferSize);
    // if (ret != 0) {
    //     emxloge("AI_APP_PersonCapture_Init failed with %#x,bufferSize=%d\n", ret, bufferSize);
    //     return;
    // }
    // AI_SetModelThreshold(m_aiHandle, m_appHandle->person_cpt_info->od_model_index, m_threshold);

    m_got = false;
    emxlogi("done\n");
}

void PassengerFlowStatistics::Stop(const Json::Value &param) {
    if (m_ena) m_job.Destroy();
    emxlogi("done\n");
}

void PassengerFlowStatistics::OnGetFrame(void *arg) {
    // VIDEO_FRAME_INFO_S stFrame = {};
    // int s32Ret = 0;
    // s32Ret = GetChnFrame(m_vpssGrp, m_vpssChn, &stFrame, 500);
    // if (s32Ret != 0) {
    //     emxloge("chn %d GetChnFrame failed with %#x\n", m_chn, s32Ret);
    //     return;
    // }

    // s32Ret = AI_APP_ConsumerCounting_Run(m_appHandle, &stFrame);
    // if (s32Ret == 0) {
    //     ai_object_t *aiObject = &m_appHandle->person_cpt_info->last_head;
    //     for (int i = 0; i < (int) aiObject->size; i++) {
    //         auto &bbox = aiObject->info[i].bbox;
    //         emxlogt("passenger info[%d]:(%f,%f) w=%f,h=%f\n", i, bbox.x1, bbox.y1, bbox.x2 - bbox.x1,
    //                 bbox.y2 - bbox.y1);
    //     }
    //     if (aiObject->entry_num != m_entryNum) {
    //         int mid_num = aiObject->entry_num - m_entryNum;
    //         m_data.msg.entryNum += mid_num;
    //         m_entryNum = aiObject->entry_num;
    //         m_got = true;
    //     }
    //     if (aiObject->miss_num != m_leaveNum) {
    //         int mid_num = aiObject->miss_num - m_leaveNum;
    //         m_data.msg.leaveNum += mid_num;
    //         m_leaveNum = aiObject->miss_num;
    //         m_got = true;
    //     }
    //     if (m_got) {
    //         auto &header = m_data.header;
    //         header.chn = m_chn;
    //         header.type = MediaInfAiData::TypeE::PassengerFlowStatistics;
    //         header.timestampMs = Time::GetMs64();
    //         header.srcPicSize.w = (int) m_width;
    //         header.srcPicSize.h = (int) m_height;
    //     }
    // }
    // s32Ret = ReleaseChnFrame(m_vpssGrp, m_vpssChn, &stFrame);
    // if (s32Ret != 0) { emxloge("chn %d ReleaseChnFrame failed with %#x\n", m_chn, s32Ret); }
}

void PassengerFlowStatistics::OnGetFrameComplete(void *arg) {
    //! 得到数据后这里才能push出去
    if (!m_got) return;
    MediaAiJob::PushAiData(&m_data);
    m_got = false;
}

ErrCodeE PassengerFlowStatistics::Ctrl(const Json::Value &in, Json::Value &out) {
    ErrCodeE e = ErrCodeE::OperationNotSupport;
    if (in.isMember("passengerFlowStatistics")) {
        if (in["passengerFlowStatistics"].isMember("clear")) {
            bool clear = in["passengerFlowStatistics"]["clear"].asBool();
            if (clear) {
                //清空客流计数
                m_data.msg.entryNum = 0;
                m_data.msg.leaveNum = 0;
                m_entryNum = 0;
                m_leaveNum = 0;
            }
        }
        e = ErrCodeE::Success;
    }
    return e;
}