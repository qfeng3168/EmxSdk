//
// Created by xiong on 2023/9/11.
//

#include "FdPdFusion.hpp"
#include "EmxCore.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

void FdPdFusion::Create(const char *workspace, Json::Value &param) {
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
    m_interval = json["interval"].asInt();
    m_threshold = json["threshold"].asInt();
    m_minArea = json["minArea"].asInt();
    m_ena = m_param["ena"].asBool();
    
    //! 开始Ai处理
    Start(m_param);
    m_created = true;
}

void FdPdFusion::Destroy() {
    //! 结束Ai处理并销毁
    if (m_created) {
        Stop(m_param);
        m_created = false;
    }
}

ErrCodeE FdPdFusion::SetParam(const Json::Value &paramNew) {
    m_param = paramNew[m_name];
    m_ena = m_param["ena"].asBool();
    // Stop(m_param);
    // Start(paramNew);
    return ErrCodeE::Success;
}

void FdPdFusion::Start(const Json::Value &param) {
    if (m_started) return;
    if (m_ena) return;
    //! 初始化模型等操作
    //! ...
    //! 使用job获取线程池资源，其中OnGetFrame运行在线程池中，OnGetFrameComplete运行在EmxMediaServer的主loop中
    m_gotPersonDetection = false;
    m_gotFaceSnap = false;
    m_job.Create(std::bind(&FdPdFusion::OnGetFrame, this, ph_1),
            std::bind(&FdPdFusion::OnGetFrameComplete, this, ph_1));
    m_started = true;
    emxlogi("done\n");
}

void FdPdFusion::Stop(const Json::Value &param) {
    //! 结束任务
    if (m_started) {
        m_job.Destroy();
        m_started = false;
        emxlogi("done\n");
    }
}


void FdPdFusion::OnGetFrame(void *arg) {
    // //! 这个函数会被循环调用，运行在线程池中，允许阻塞操作
    //    int s32Ret = 0;
    //    s32Ret = ChipVpssGetChnFrame(m_vpssGrp, m_vpssChn, &stFrame, 500);
    //    if (s32Ret != 0) {
    //        emxloge("chn %d ChipVpssGetChnFrame failed with %#x\n", m_chn, s32Ret);
    //        return;
    //    }
    //    ChipAiFdPdFusion(&stFrame, &obj, m_threshold, m_minArea);
    //    if (obj.size == 0)
    //        return;
    // // ! 如果识别到了人形
    // if (obj.hasPersonDetection) {
    //     auto &header = m_dataPersonDetection.header;
    //     header.chn = m_chn;
    //     header.type = MediaInfAiData::TypeE::PersonDetection;
    //     header.timestampMs = Time::GetMs64();
    //     header.srcPicSize.w = (int) stFrame.stVFrame.u32Width;
    //     header.srcPicSize.h = (int) stFrame.stVFrame.u32Height;
    //     m_dataPersonDetection.num = (int) obj.size;
    //     m_dataPersonDetection.Alloc(m_data.num);
    //     for (int i = 0; i < (int) obj.size; i++) {
    //         auto &bbox = obj.info[i].bbox;
    //         m_data.array[i].Set((int) bbox.x1, (int) bbox.y1, (int) (bbox.x2 - bbox.x1), (int) (bbox.y2 - bbox.y1));
    //     }
    //     m_gotPersonDetection = true;
    // }
    // // ! 如果抓拍到了人脸
    // if (obj.hasFaceSnap) {
    //     auto &header = m_dataFaceSnap.header;
    //     header.chn = m_chn;
    //     header.type = MediaInfAiData::TypeE::FaceSnap;
    //     header.timestampMs = Time::GetMs64();
    //     header.srcPicSize.w = (int) stFrame.stVFrame.u32Width;
    //     header.srcPicSize.h = (int) stFrame.stVFrame.u32Height;
    //     m_dataFaceSnap.num = (int) obj.size;
    //     m_dataFaceSnap.Alloc(m_data.num);
    //     for (int i = 0; i < (int) obj.size; i++) {
    //         auto &bbox = obj.info[i].bbox;
    //         m_data.array[i].Set((int) bbox.x1, (int) bbox.y1, (int) (bbox.x2 - bbox.x1), (int) (bbox.y2 - bbox.y1));
    //     }
    //     m_gotFaceSnap = true;
    // }
    // ChipAiFree(&obj);
    // s32Ret = ChipVpssReleaseChnFrame(m_vpssGrp, m_vpssChn, &stFrame);
    // if (s32Ret != 0) { emxloge("chn %d ChipVpssReleaseChnFrame failed with %#x\n", m_chn, s32Ret); }
}

void FdPdFusion::OnGetFrameComplete(void *arg) {
    //! 得到数据后这里才能push出去
    if (m_gotPersonDetection) {
        MediaAiJob::PushAiData(&m_dataPersonDetection);
        m_dataPersonDetection.Free();
        m_gotPersonDetection = false;
    }
    if (m_gotFaceSnap) {
        MediaAiJob::PushAiData(&m_dataFaceSnap);
        m_dataFaceSnap.Free();
        m_gotFaceSnap = false;
    }
}

ErrCodeE FdPdFusion::FaceRegister(const Json::Value &info, Json::Value &out) {
    ErrCodeE e = ErrCodeE::Success;
    //人脸提取特征数据
    for (int i = 0; i < (int) info.size(); i++) {
        Json::Value faceInfo;
        std::string buf;
        faceInfo["id"] = info[i]["id"].asInt();
        //是使用jpg图片提取
        if(info["isImg"].asBool()){
             //e = getFeature(info[i]["path"].asString(),buf);
            emxlogi("register[%d],id=%d,path=%s\n", i,info[i]["id"].asInt(),info[i]["path"].asString().c_str());
        }else{
             //e = getSourceFeature(buf);
              emxlogi("register[%d],id=%d,path=null\n", i,info[i]["id"].asInt());
        }
        if(e ==  ErrCodeE::Success){
            std::string feature;
            Base64::Encode(buf, feature);
            faceInfo["feature"] = feature;
            out.append(faceInfo);
        }
    }
    return e;
}

ErrCodeE FdPdFusion::FaceUnregister(const Json::Value &id) {
    for (int i = 0; i < (int) id.size(); i++) {
         int idNum = id[i].asInt();
        emxlogi("unregister[%d],id=%d\n", i,idNum);
        //删除对应id人脸库
        // faceDelete(idNum);
    }
    return ErrCodeE::Success;
}

ErrCodeE FdPdFusion::FaceSyncLibrary(const Json::Value &library) {
    //同步特征数据
    for (int i = 0; i < (int) library.size(); i++) {
        int idNum = library[i]["id"].asInt();
        std::string feature = library[i]["feature"].asString();
        //添加人脸库
        std::string buf;
        Base64::Decode(feature,buf);
        emxlogi("lirary[%d] idNum=%d\n", i,idNum);
        // addLibrary(idNum,buf);
    }
    return ErrCodeE::Success;
}

ErrCodeE FdPdFusion::Ctrl(const Json::Value &in, Json::Value &out) {
    ErrCodeE e = ErrCodeE::Success;
    if (in.isMember("faceRegister")) { e = FaceRegister(in["faceRegister"], out); }
    if (in.isMember("faceUnregister")) { e = FaceUnregister(in["faceUnregister"]); }
    if (in.isMember("syncLibrary")) { e = FaceSyncLibrary(in["syncLibrary"]); }
    return e;
}