//
// Created by tangfahong on 2024/4/1.
//

#include "CrossingInvasionComm.hpp"
#include "EmxCore.hpp"

using namespace Emx;

void CrossingInvasionComm::Create(const char *workspace, Json::Value &param) {
    if (m_created) return;
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
    emxlogd("m_modePath = %s\n", m_modePath.c_str());

    //! 开始Ai处理
    Start();
    m_start = false;
    m_created = true;
    SetParam(param);
}

void CrossingInvasionComm::Destroy() { Stop(); }

int CrossingInvasionComm::Start() {
    int ret = 0;
    uint32_t bufferSize = 10;
    /*创建算法句柄*/
    // ret = AI_CreateHandle(&m_aiHandle);
    // if (ret != SUCCESS) {
    //     emxloge("handle create failed with %#x!\n", ret);
    //     return ret;
    // }
    // ret = AI_APP_CreateHandle(&m_appHandle, m_aiHandle);
    // if (ret != 0) {
    //     emxloge("AI_APP_CreateHandle failed with %#x\n", ret);
    //     return ret;
    // }
    /*加载模型*/
    // ret = AI_APP_PersonVehicleCapture_QuickSetUp(m_appHandle, "yolov8", m_modePath.c_str(), NULL);
    // if (ret != 0) {
    //     emxloge("AI_APP_PersonVehicleCapture_QuickSetUp failed with %#x,modelPath=%s\n", ret, m_modePath.c_str());
    //     return ret;
    // }
    m_crossingGot = false;
    m_invasionGot = false;
    emxlogi("done\n");
    return ret;
}


bool CrossingInvasionComm::TypeFiltration(int type, int classes) {
    int alarm_type = 0;
    if (classes == 0) {
        alarm_type = 0x1;
    } else if (classes == 1) {
        alarm_type = 0x2;
    } else if (classes == 2) {
        alarm_type = 0x4;
    }
    if ((type & alarm_type) == alarm_type) { return true; }
    return false;
}

bool CrossingInvasionComm::AreaCheck(ai_bbox_t &rect, uint32_t srcWidth, uint32_t srcHeight) {
    int width = abs(rect.x2 - rect.x1);
    int height = abs(rect.y2 - rect.y1);
    int area = width * height;

    int src_area = srcWidth * srcHeight;
    if (area > (src_area * 2 / 3)) { return false; }
    return true;
}


bool CrossingInvasionComm::InvadeMap(ai_bbox_t &rect, uint32_t width, uint32_t height) {
    int reg_x, reg_y;
    int x_ratio = width / mapWidth;
    int y_ratio = height / mapHeight;
    int region_Left =
            rect.x1 / x_ratio; /*左右的范围，除以16是满足mdcfg.map mapWidth*mapHeight 的矩阵，640/16=mapWidth*/
    int region_Right = rect.x2 / x_ratio;
    int region_Top =
            rect.y1 / y_ratio; /*上下的范围，除以12是满足mdcfg.map mapWidth*mapHeight 的矩阵，360/12=mapHeight*/
    int region_Bottom = rect.y2 / y_ratio;
    if (region_Left > mapWidth) region_Left = mapWidth;
    if (region_Right > mapWidth) region_Right = mapWidth;
    if (region_Top > mapHeight) region_Right = mapHeight;
    if (region_Bottom > mapHeight) region_Bottom = mapHeight;

    //判断目标面积与原始检测区域占比，占比大则误检
    if (!AreaCheck(rect, width, height)) return false;

    /*判断范围侦测出的区域是否在配置区域*/
    for (reg_y = region_Top; reg_y < region_Bottom; reg_y++) {
        for (reg_x = region_Left; reg_x < region_Right; reg_x++) {
            if (m_invasionMap[mapWidth * reg_y + reg_x]) { return true; }
        }
    }
    return false;
}

void CrossingInvasionComm::InvasionFrameComplete(void *aiObject, void *pstFrame) {
#if 0
    int invasionNum = 0;
    for (int i = 0; i < (int) aiObject->size; i++) {
        cvai_bbox_t &bbox = aiObject->info[i].bbox;
        if (TypeFiltration(m_invasionType, aiObject->info[i].classes)) {
            if (InvadeMap(bbox, aiObject->width, aiObject->height)) invasionNum++;
        }
    }
    if (invasionNum > 0) {
        auto &header = m_invasionData.header;
        header.chn = m_chn;
        header.type = MediaInfAiData::TypeE::InvasionDetection;
        header.timestampMs = Time::GetMs64();
        header.srcPicSize.w = (int) aiObject->width;
        header.srcPicSize.h = (int) aiObject->height;
        m_invasionData.num = invasionNum;
        m_invasionData.Alloc(m_invasionData.num);
        int indx = 0;
        for (int i = 0; i < (int) aiObject->size; i++) {
            cvai_bbox_t &bbox = aiObject->info[i].bbox;
            if (TypeFiltration(m_invasionType, aiObject->info[i].classes)) {
                if (InvadeMap(bbox, aiObject->width, aiObject->height)) {
                    emxlogt("invasion info[%d]:(%f,%f) w=%f,h=%f\n", i, bbox.x1, bbox.y1, bbox.x2 - bbox.x1,
                            bbox.y2 - bbox.y1);
                    m_invasionData.array[indx].id = aiObject->info[i].unique_id;
                    if (aiObject->info[i].classes == 0) {
                        m_invasionData.array[indx].type = MediaInfAiDataInvasionDetection::DataMsg::TypeE::Person;
                    } else if (aiObject->info[i].classes == 1) {
                        m_invasionData.array[indx].type = MediaInfAiDataInvasionDetection::DataMsg::TypeE::Vehicle;
                    } else if (aiObject->info[i].classes == 2) {
                        m_invasionData.array[indx].type =
                                MediaInfAiDataInvasionDetection::DataMsg::TypeE::NonMotorVehicle;
                    }
                    m_invasionData.array[indx].rect.Set((int) bbox.x1, (int) bbox.y1, (int) (bbox.x2 - bbox.x1),
                            (int) (bbox.y2 - bbox.y1));
                    //todo：pic
                    m_invasionData.array[indx].pic.size = 0;
                    indx++;
                }
            }
        }
        m_invasionGot = true;
    }
#endif
}

void CrossingInvasionComm::CrossingFrameComplete(cvai_object_t *aiObject, VIDEO_FRAME_INFO_S *pstFrame) {
#if 0
    int crossNum = 0;
    for (int i = 0; i < (int) aiObject->size; i++) {
        if (aiObject->info[i].is_cross) {
            if (TypeFiltration(m_crossType, aiObject->info[i].classes)) crossNum++;
        }
    }
    if (crossNum > 0) {
        auto &header = m_crossingData.header;
        header.chn = m_chn;
        header.type = MediaInfAiData::TypeE::CrossingDetection;
        header.timestampMs = Time::GetMs64();
        header.srcPicSize.w = (int) aiObject->width;
        header.srcPicSize.h = (int) aiObject->height;
        m_crossingData.num = crossNum;
        m_crossingData.Alloc(m_crossingData.num);
        int indx = 0;
        for (int i = 0; i < (int) aiObject->size; i++) {
            if (aiObject->info[i].is_cross) {
                if (TypeFiltration(m_crossType, aiObject->info[i].classes)) {
                    auto &bbox = aiObject->info[i].bbox;
                    emxlogt("cross info[%d]:(%f,%f) w=%f,h=%f\n", i, bbox.x1, bbox.y1, bbox.x2 - bbox.x1,
                            bbox.y2 - bbox.y1);
                    m_crossingData.array[indx].id = aiObject->info[i].unique_id;
                    if (aiObject->info[i].classes == 0) {
                        m_crossingData.array[indx].type = MediaInfAiDataCrossingDetection::DataMsg::TypeE::Person;
                    } else if (aiObject->info[i].classes == 1) {
                        m_crossingData.array[indx].type = MediaInfAiDataCrossingDetection::DataMsg::TypeE::Vehicle;
                    } else if (aiObject->info[i].classes == 2) {
                        m_crossingData.array[indx].type =
                                MediaInfAiDataCrossingDetection::DataMsg::TypeE::NonMotorVehicle;
                    }
                    m_crossingData.array[indx].rect.Set((int) bbox.x1, (int) bbox.y1, (int) (bbox.x2 - bbox.x1),
                            (int) (bbox.y2 - bbox.y1));
                    //todo：pic

                    m_crossingData.array[indx].pic.size = 0;
                    indx++;
                }
            }
        }
        m_crossingGot = true;
    }
#endif
}

void CrossingInvasionComm::OnGetFrame(void *arg) {
    /*获取数据流并且识别*/
    // VIDEO_FRAME_INFO_S stFrame = {};
    // int s32Ret = SUCCESS;
    // s32Ret = VPSS_GetChnFrame(m_vpssGrp, m_vpssChn, &stFrame, 2000);
    // if (s32Ret != SUCCESS) {
    //     emxloge("chn %d VPSS_GetChnFrame failed with %#x\n", m_chn, s32Ret);
    //     return;
    // }
    // s32Ret = AI_APP_PersonVehicleCapture_Run(m_appHandle, &stFrame);
    // if (s32Ret == SUCCESS) {
    //     VIDEO_FRAME_INFO_S *pstFrame = &stFrame;
    //     cvai_object_t *aiObject = &m_appHandle->personvehicle_cpt_info->last_objects;
    //     if (aiObject->size > 0) {
    //         if (m_invasionEna) InvasionFrameComplete(aiObject, pstFrame);
    //         if (m_crossEna) CrossingFrameComplete(aiObject, pstFrame);
    //     }
    // }
    // s32Ret = VPSS_ReleaseChnFrame(m_vpssGrp, m_vpssChn, &stFrame);
    // if (s32Ret != SUCCESS) { emxloge("chn %d VPSS_ReleaseChnFrame failed with %#x\n", m_chn, s32Ret); }
}

void CrossingInvasionComm::OnGetFrameComplete(void *arg) {
    if (m_crossingGot) {
        MediaAiJob::PushAiData(&m_crossingData);
        m_crossingData.Free();
        m_crossingGot = false;
    }
    if (m_invasionGot) {
        MediaAiJob::PushAiData(&m_invasionData);
        m_invasionData.Free();
        m_invasionGot = false;
    }
}

void CrossingInvasionComm::Stop() {
    if (m_created) {
        if (m_start) { m_job.Destroy(); }
        // AI_APP_DestroyHandle(m_appHandle);
        // AI_DestroyHandle(m_aiHandle);
        m_created = false;
    }
    emxlogd("destroy done\n");
}

ErrCodeE CrossingInvasionComm::CrossingParamSet(const Json::Value &params) {
    m_crossType = params["crossType"].asInt();
    m_crossEna = params["ena"].asBool();

    int lineX1 = params["lineX1"].asInt();
    int lineY1 = params["lineY1"].asInt();
    int lineX2 = params["lineX2"].asInt();
    int lineY2 = params["lineY2"].asInt();

    lineX1 = lineX1 * (int) m_width / 10000;
    lineY1 = lineY1 * (int) m_height / 10000;
    lineX2 = lineX2 * (int) m_width / 10000;
    lineY2 = lineY2 * (int) m_height / 10000;


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
    int crossStyle = params["direction"].asInt();
    int statisticsMode = 2;
    if (crossStyle == 1) {
        statisticsMode = 1;
    } else if (crossStyle == 0) {
        statisticsMode = 0;
    }
    emxlogi("ConsumerCounting_Line(%d,%d)->(%d,%d)!statisticsMode=%d\n", lineX1, lineY1, lineX2, lineY2,
            statisticsMode);
    // int ret = AI_APP_PersonVehicleCapture_Line(m_appHandle, lineX1, lineY1, lineX2, lineY2, statisticsMode);
    // if (ret != 0) {
    //     emxloge("AI_APP_PersonVehicleCapture_Line err ,ret=%d!\n", ret);
    //     return ErrCodeE::IllegalParam;
    // }
    return ErrCodeE::Success;
}

ErrCodeE CrossingInvasionComm::InvasionParamSet(const Json::Value &params) {
    m_invasionType = params["invasionType"].asInt();
    m_invasionEna = params["ena"].asBool();

    memset(m_invasionMap, 0, mapSizeMax);
    for (int i = 0; i < (int) params["map"].size(); i++) { m_invasionMap[i] = (uint8_t) params["map"][i].asInt(); }
    return ErrCodeE::Success;
}

ErrCodeE CrossingInvasionComm::SetParam(const Json::Value &paramNew) {
    if (!m_created) return ErrCodeE::ResNotExist;
    ErrCodeE e;
    if (paramNew["CrossingDetection"].isObject()) {
        e = CrossingParamSet(paramNew["CrossingDetection"]);
        if (e != ErrCodeE::Success) { return e; }
    }
    if (paramNew["InvasionDetection"].isObject()) {
        e = InvasionParamSet(paramNew["InvasionDetection"]);
        if (e != ErrCodeE::Success) { return e; }
    }
    bool start = false;
    if (m_invasionEna || m_crossEna) { start = true; }
    if (start != m_start) {
        if (!m_start) {
            m_job.Create(std::bind(&CrossingInvasionComm::OnGetFrame, this, ph_1),
                    std::bind(&CrossingInvasionComm::OnGetFrameComplete, this, ph_1), nullptr, "CrossingInvasion");
        } else {
            m_job.Destroy();
        }
        m_start = start;
    }
    return ErrCodeE::Success;
}
