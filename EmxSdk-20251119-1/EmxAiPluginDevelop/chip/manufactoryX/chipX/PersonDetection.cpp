//
// Created by xiong on 2023/9/11.
//

#include "PersonDetection.hpp"
#include "EmxCore.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

void PersonDetection::Create(const char *workspace, Json::Value &param) {
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
    m_vpssGrp = json["vpssGrp"].asInt();
    m_vpssChn = json["vpssChn"].asInt();
    m_threshold = json["threshold"].asFloat();
    m_modelId = json["modelId"].asInt();

    m_modePath = SysEnv::GetDataDir();
    m_modePath.append("/AiPlugin/");
    m_modePath.append(json["modelPath"].asCString());
    m_ena = false;
    emxlogd("m_modelId=%d,m_threshold=%f,m_modePath=%s,ena=%d\n", m_modelId,
            m_threshold, m_modePath.c_str(),m_ena);
    //! 开始Ai处理
    Start(m_param);
    m_created = true;
    SetParam(m_param);
    emxlogi("done\n");
}

void PersonDetection::Destroy() {
    //! 结束Ai处理并销毁
    if (m_created) {
        Stop(m_param);
        m_created = false;
    }
}

ErrCodeE PersonDetection::SetParam(const Json::Value &paramNew) {
    if (!paramNew[m_name].isObject()) {
        return ErrCodeE::Success;
    }
    m_param = paramNew[m_name];
    bool enable = m_param["ena"].asBool();
    if (enable != m_ena) {
        if (m_ena) {
            m_job.Destroy();
        } else {
            m_job.Create(std::bind(&PersonDetection::OnGetFrame, this, ph_1),
                    std::bind(&PersonDetection::OnGetFrameComplete, this, ph_1), nullptr, "PersonDetection");
        }
        m_ena = enable;
    }

    return ErrCodeE::Success;
}

void PersonDetection::Start(const Json::Value &param) {
    //! 初始化模型等操作
    //! ...
    //! 使用job获取线程池资源，其中OnGetFrame运行在线程池中，OnGetFrameComplete运行在EmxMediaServer的主loop中
    m_got = false;
    emxlogi("done\n");
}

void PersonDetection::Stop(const Json::Value &param) {
    //! 结束任务
    if (m_ena) { m_job.Destroy(); }
    emxlogi("done\n");
}

void PersonDetection::OnGetFrame(void *arg) {
    //! 这个函数会被循环调用，运行在线程池中，允许阻塞操作
    //    int s32Ret = 0;
    //    s32Ret = ChipVpssGetChnFrame(m_vpssGrp, m_vpssChn, &stFrame, 500);
    //    if (s32Ret != 0) {
    //        emxloge("chn %d ChipVpssGetChnFrame failed with %#x\n", m_chn, s32Ret);
    //        return;
    //    }
    //    ChipAiPersonDetection(&stFrame, &obj, m_threshold, m_minArea);
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
	// 	  m_got = true;
    m_got = true;
}

void PersonDetection::OnGetFrameComplete(void *arg) {
    //! 得到数据后这里才能push出去
    if (!m_got) return;
    MediaAiJob::PushAiData(&m_data);
    m_data.Free();
    m_got = false;
}
