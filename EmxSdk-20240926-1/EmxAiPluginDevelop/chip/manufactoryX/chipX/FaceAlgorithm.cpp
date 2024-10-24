//
// Created by xiong on 2023/9/11.
//

#include "FaceAlgorithm.hpp"
#include "EmxCore.hpp"
#include "media/server/MediaServerCtx.hpp"

using namespace Emx;

void FaceAlgorithm::Create(const char *workspace, Json::Value &param) {
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
    std::string modeDir = SysEnv::GetDataDir();
    modeDir += "/AiPlugin/";
    auto &json = root[m_chn];
    m_vbId = json["vbId"].asInt();
    m_vpssGrp = json["vpssGrp"].asInt();
    m_vpssChn = json["vpssChn"].asInt();
    m_thresholdFace = json["thresholdFace"].asFloat();
    m_snapModelPath = modeDir + json["snapModelPath"].asCString();
    m_recognitionModelPath = modeDir + json["recognitionModelPath"].asCString();
    m_maskModelPath = modeDir + json["maskModelPath"].asCString();

    //! 开始Ai处理
    Start(param);
}

void FaceAlgorithm::Destroy() {
    //! 结束Ai处理并销毁
    if (m_created) {
        Stop();
        m_created = false;
    }
}

ErrCodeE FaceAlgorithm::SetFaceParam(const Json::Value &param) {
    // int ret = 0;    
    m_faceRecognition.ena = param["ena"].asBool();                             //!<人脸抓拍开关
    m_faceRecognition.captureBackImg = param["captureBackImg"].asBool();       //!<人脸抓拍背景大图
    m_faceRecognition.captureRectImg = param["captureRectImg"].asBool();       //!<人脸抓拍抠图
    m_faceRecognition.aeEna = param["aeEna"].asBool();                         //!<人脸抓拍曝光控制
    m_faceRecognition.maskEna = param["maskEna"].asBool();                     //!<口罩检测开关
    m_faceRecognition.recognitionEna = param["recognitionEna"].asBool();       //!<人脸识别比对开关
    m_faceRecognition.maskThreshold = (uint8_t) param["maskThreshold"].asInt();//!<口罩检测阈值
    m_faceRecognition.aeThreshold = (uint8_t) param["aeThreshold"].asInt();    //!<人脸抓拍曝光等级[0~255]
    m_faceRecognition.threshold = (uint8_t) param["threshold"].asInt();        //!<人脸抓拍阈值[0~100]
    m_faceRecognition.snapMode = (uint8_t) param["snapMode"].asInt(); //!<人脸抓拍模式 0=快速,1=质量,2=定时
    m_faceRecognition.snapTime = (uint16_t) param["snapTime"].asInt();//!<人脸抓拍间隔时间，一般是帧数
    m_faceRecognition.snapQuality = (uint8_t) param["snapQuality"].asInt();      //!<人脸抓拍质量等级[0~100]
    m_faceRecognition.snapClarity = (uint8_t) param["snapClarity"].asInt();      //!<人脸抓拍清晰度阈值[0~100]
    m_faceRecognition.snapAngle = (uint8_t) param["snapAngle"].asInt();          //!<人脸抓拍角度限制[0~255]
    m_faceRecognition.captureQuality = (uint8_t) param["captureQuality"].asInt();//!<人脸抓拍图片质量[0~100]
    m_faceRecognition.minWidth = (uint16_t) param["minWidth"].asInt();           //!<人脸抓拍最小像素宽
    m_faceRecognition.minHeight = (uint16_t) param["minHeight"].asInt();         //!<人脸抓拍最小像素高

    /*人脸检测阈值设置*/
    // ret = AI_SetModelThreshold(m_aiHandle, m_faceRecognition.threshold);
    // if (ret != 0) {
    //     emxloge("Set face snap threshold[%d] failed with %#x!\n", m_faceRecognition.threshold, ret);
    // }
    /*设置人脸抓拍模式*/
    // ret = AI_APP_FaceCapture_SetMode(m_appHandle, m_faceRecognition.snapMode);
    // if (ret != 0) { emxloge("set mode[%d] failed with %#x!\n", m_faceRecognition.snapMode, ret); }
    /*设置人脸其他参数*/

    emxlogd("Set face param succeed!\n");
    return ErrCodeE::Success;
}

ErrCodeE FaceAlgorithm::SetFaceDetectionParam(const Json::Value &param) {
    m_faceDetection.ena = param["ena"].asBool();
    m_faceDetection.minWidth = (uint16_t) param["minWidth"].asInt();
    m_faceDetection.minHeight = (uint16_t) param["minHeight"].asInt();
    emxlogd("Set face detection param succeed!\n");
    return ErrCodeE::Success;
}

ErrCodeE FaceAlgorithm::SetParam(const Json::Value &paramNew) {
    if (!m_created) return ErrCodeE::ResNotExist;
    ErrCodeE e = ErrCodeE::Success;
    if (paramNew.isMember("FaceRecognition")) { e = SetFaceParam(paramNew["FaceRecognition"]); }
    if (paramNew.isMember("FaceDetection")) { e = SetFaceDetectionParam(paramNew["FaceDetection"]); }
    return e;
}

int FaceAlgorithm::FaceModelInit() {
    // int ret = 0;
    /*创建ai句柄 */
    // ret = AI_CreateHandle(&m_aiHandle);
    // if (ret != 0) {
    //     emxloge("handle create failed with %#x!\n", ret);
    //     return ret;
    // }
    // ret = AI_APP_CreateHandle(&m_appHandle, m_aiHandle);
    // if (ret != 0) {
    //     emxloge("set FaceCapture failed with %#x!\n", ret);
    //     AI_DestroyHandle(m_aiHandle);
    //     return ret;
    // }

    /*加载人脸检测和识别模型*/
    // ret = AI_Face_OpenModel(m_appHandle, m_snapModelPath.c_str(), m_recognitionModelPath.c_str());
    // if (ret != 0) {
    //     emxloge("Face model initialization failed  with %#x!\n", ret);
    //     return ret;
    // }
    // /*加载口罩模型*/
    // ret = AI_Mask_OpenModel(m_aiHandle, m_maskModelPath.c_str());
    // if (ret != 0) {
    //     emxloge("Mask model initialization failed  with %#x!\n", ret);
    //     return ret;
    // }
    return 0;
}

void FaceAlgorithm::Start(const Json::Value &param) {
    if (m_started) return;
    int ret = 0;
    //! 初始化模型等操作
    ret = FaceModelInit();
    if (ret) {
        emxloge("FaceModelInit failed with %#x!\n", ret);
        return;
    }
    m_created = true;
    /*设置参数*/
    SetParam(param);
    /*初始化发送标志*/
    m_gotFaceRecognition = false;
    m_gotFaceDetection = false;
    /*创建检测loop*/
    m_job.Create(std::bind(&FaceAlgorithm::OnGetFrame, this, ph_1),
            std::bind(&FaceAlgorithm::OnGetFrameComplete, this, ph_1));
    m_started = true;
    emxlogi("done\n");
}

void FaceAlgorithm::Stop() {
    //! 结束任务
    if (m_started) {
        m_job.Destroy();
        m_started = false;
        emxlogi("done\n");
    }
}


int FaceAlgorithm::FaceAeCtrl(void *faces, void *pstFrame) {
    int ret = 0;
    //根据人脸信息来控制ae
    return ret;
}


int FaceAlgorithm::FaceManage(void *pstFrame) {
    // ai_face_t *pFaceLast = &m_appHandle->face_cpt_info->last_faces;
    // face_capture_t *pFaceCptInfo = m_appHandle->face_cpt_info;
    // if (pFaceLast->size > 0) {
    //     //ae 处理
    //     if (m_faceRecognition.aeEna) { FaceAeCtrl(pFaceLast, pstFrame); }
    // }
    int indx = 0;         //<!实际人脸数组下标统计
    /*先统计有效数据数量*/
#if 0
    int fullImgindex = -1;//<! 背景图抓取的下标
    int faceNum = 0;      //<!实际人脸数量
    for (uint32_t i = 0; i < pFaceCptInfo->size; ++i) {
        if (!pFaceCptInfo->_output[i]) { continue; }
        if ((fullImgindex == -1) && pFaceCptInfo->data[i].image.full_img &&
                (pFaceCptInfo->data[i].image.full_length > 0)) {
            fullImgindex = i;
        }
        faceNum++;
    }
    if (faceNum > 0) {
        auto &header = m_dataRecognition.header;
        header.chn = m_chn;
        header.type = MediaInfAiData::TypeE::FaceRecognition;
        header.timestampMs = Time::GetMs64();
        header.srcPicSize.w = (int) pstFrame->stVFrame.u32Width;
        header.srcPicSize.h = (int) pstFrame->stVFrame.u32Height;
        m_dataRecognition.num = faceNum;
        m_dataRecognition.Alloc(m_dataRecognition.num);

        if ((fullImgindex >= 0) && (m_faceRecognition.captureBackImg)) {
            if (pFaceCptInfo->data[fullImgindex].image.full_img &&
                    pFaceCptInfo->data[fullImgindex].image.full_length > 0) {
                m_dataRecognition.fullPic.img = (uint8_t *) EP_MALLOC(pFaceCptInfo->data[fullImgindex].image.full_length);
                if (m_dataRecognition.fullPic.img) {
                    memcpy(m_dataRecognition.fullPic.img, pFaceCptInfo->data[fullImgindex].image.full_img,
                            pFaceCptInfo->data[fullImgindex].image.full_length);
                    m_dataRecognition.fullPic.type = MediaInfAiPic::TypeE::Jpg;
                    m_dataRecognition.fullPic.size = pFaceCptInfo->data[fullImgindex].image.full_length;
                    m_dataRecognition.fullPic.width = pstFrame->stVFrame.u32Width;
                    m_dataRecognition.fullPic.height = pstFrame->stVFrame.u32Height;
                }
            }
        }
        for (int i = 0; i < (int) pFaceCptInfo->size; ++i) { /*抓图开始*/
            if (!pFaceCptInfo->_output[i]) { continue; }
            cvai_bbox_t *pBbox = &pFaceCptInfo->data[i].info.bbox;
            emxlogd("unique_id:%llu imgae[w:%u h:%u stride:(%u %u %u) quality:%f score:%f length:(%u %u %u)]\n",
                    pFaceCptInfo->data[i].info.unique_id, pFaceCptInfo->data[i].image.width,
                    pFaceCptInfo->data[i].image.height, pFaceCptInfo->data[i].image.stride[0],
                    pFaceCptInfo->data[i].image.stride[1], pFaceCptInfo->data[i].image.stride[2],
                    pFaceCptInfo->data[i].info.face_quality, pFaceCptInfo->data[i].info.bbox.score,
                    pFaceCptInfo->data[i].image.length[0], pFaceCptInfo->data[i].image.length[1],
                    pFaceCptInfo->data[i].image.length[2]);
            m_dataRecognition.array[indx].id = pFaceCptInfo->data[i].info.unique_id;
            m_dataRecognition.array[indx].captureTime = pFaceCptInfo->data[i].cap_timestamp;
            m_dataRecognition.array[indx].startTime = pFaceCptInfo->data[i].last_cap_timestamp;
            m_dataRecognition.array[indx].endTime = pFaceCptInfo->data[i]._timestamp;
            m_dataRecognition.array[indx].quality = (uint16_t) pFaceCptInfo->data[i].info.face_quality;
            m_dataRecognition.array[indx].sharpness = pFaceCptInfo->data[i].info.sharpness_score;
            m_dataRecognition.array[indx].ipd = 0;
            m_dataRecognition.array[indx].age = (uint8_t) pFaceCptInfo->data[i].info.age;
            m_dataRecognition.array[indx].gender = (uint8_t) pFaceCptInfo->data[i].info.gender;
            m_dataRecognition.array[indx].emotion = (uint8_t) pFaceCptInfo->data[i].info.emotion;
            m_dataRecognition.array[indx].skin = (uint8_t) pFaceCptInfo->data[i].info.race;
            if (pFaceCptInfo->data[i].info.liveness_score > 0.5) {
                m_dataRecognition.array[indx].liveness = 1;
            } else if (pFaceCptInfo->data[i].info.liveness_score == 0.0) {
                m_dataRecognition.array[indx].liveness = 0;
            } else {
                m_dataRecognition.array[indx].liveness = 2;
            }
            if (pFaceCptInfo->data[i].info.hardhat_score > 0.5) {
                m_dataRecognition.array[indx].hat = 1;
            } else if (pFaceCptInfo->data[i].info.hardhat_score == 0.0) {
                m_dataRecognition.array[indx].hat = 0;
            } else {
                m_dataRecognition.array[indx].hat = 2;
            }
            if ((pFaceCptInfo->data[i].info.mask_score * 100) > m_faceRecognition.maskThreshold) {
                m_dataRecognition.array[indx].isMask = 1;
            } else {
                m_dataRecognition.array[indx].isMask = 0;
            }

            m_dataRecognition.array[indx].rect.Set(pBbox->x1, pBbox->y1, pBbox->x2 - pBbox->x1, pBbox->y2 - pBbox->y1);

            if (m_faceRecognition.captureRectImg) {
                Raw2JpegInfo rawInfo = {};
                rawInfo.width = pFaceCptInfo->data[i].image.width;
                rawInfo.height = pFaceCptInfo->data[i].image.height;
                rawInfo.stride = rawInfo.width * 3;
                rawInfo.pixel_format = RAW2JPEG_MPDE_RGB;
                rawInfo.jpeg_quality = m_faceRecognition.captureQuality;
                rawInfo.src_size = pFaceCptInfo->data[i].image.length[0];
                rawInfo.src_buffer = pFaceCptInfo->data[i].image.pix[0];
                unsigned long size = 0;
                Raw2Jpeg::RawToJpeg(&rawInfo, &m_dataRecognition.array[indx].pic.img, size);
                m_dataRecognition.array[indx].pic.size = (int32_t) size;
                m_dataRecognition.array[indx].pic.width = rawInfo.width;
                m_dataRecognition.array[indx].pic.height = rawInfo.height;
                m_dataRecognition.array[indx].pic.type = MediaInfAiPic::TypeE::Jpg;
            }
            //人脸识别信息处理
            if (m_faceRecognition.recognitionEna) {
                int libId = 0;
                //fload similarity =  AI_Face_Comparison(pFaceCptInfo->data[i].feature,&libId);
                m_dataRecognition.array[indx].similarity = (int)similarity * 100;
                m_dataRecognition.array[indx].libId = libId;
            }
            indx++;
        }
        m_gotFaceRecognition = true;
    }
#endif
    return indx;
}

void FaceAlgorithm::FaceDetectionManage() {
#if 0
    ai_face_t *pObj = &m_appHandle->face_cpt_info->last_faces;
    if (pObj->size <= 0) { return; }
    int faceNum = 0;//<!实际人脸数量
    int indx = 0;   //<!实际人脸数组下标统计
    /*先统计有效数据数量*/
    for (uint32_t i = 0; i < pObj->size; ++i) {
        auto &bbox = pObj->info[i].bbox;
        int width = bbox.x2 - bbox.x1;
        int height = bbox.y2 - bbox.y1;
        if ((width >= m_faceDetection.minWidth) && (height >= m_faceDetection.minHeight)) { faceNum++; }
    }
    if (faceNum > 0) {
        auto &header = m_dataFaceDetection.header;
        header.chn = m_chn;
        header.type = MediaInfAiData::TypeE::FaceDetection;
        header.timestampMs = Time::GetMs64();
        header.srcPicSize.w = (int) pObj->width;
        header.srcPicSize.h = (int) pObj->height;
        m_dataFaceDetection.num = (int) pObj->size;
        m_dataFaceDetection.Alloc(m_dataFaceDetection.num);
        for (int i = 0; i < (int) pObj->size; i++) {
            auto &bbox = pObj->info[i].bbox;
            int width = bbox.x2 - bbox.x1;
            int height = bbox.y2 - bbox.y1;
            if ((width >= m_faceDetection.minWidth) && (height >= m_faceDetection.minHeight)) {
                m_dataFaceDetection.array[indx].Set((int) bbox.x1, (int) bbox.y1, width, height);
                emxlogt("Face detection info[%d]:(%f,%f) w=%d,h=%d\n", i, bbox.x1, bbox.y1, width, height);
                indx++;
            }
        }
        m_gotFaceDetection = true;
    }
#endif
}

void FaceAlgorithm::OnGetFrame(void *arg) {
    // //! 这个函数会被循环调用，运行在线程池中，允许阻塞操作
    // int s32Ret = 0;
    // VIDEO_FRAME_INFO_S stFrame = {};
    // s32Ret = VPSS_GetChnFrame(m_vpssGrp, m_vpssChn, &stFrame, 2000);
    // if (s32Ret != 0) {
    //     emxloge("chn %d VPSS_GetChnFrame failed with %#x\n", m_chn, s32Ret);
    //     return;
    // }
    // s32Ret = AI_APP_FaceCapture_Run(m_appHandle, &stFrame);
    // if (s32Ret != 0) {
    //     emxloge("AI_APP_FaceCapture_Run fail with %#x \n", s32Ret);
    //     VPSS_ReleaseChnFrame(m_vpssGrp, m_vpssChn, &stFrame);
    //     return;
    // }
    // if (m_faceRecognition.maskEna) {
    //     s32Ret = AI_MaskClassification(m_aiHandle, &stFrame, &m_appHandle->face_cpt_info->last_faces);
    //     if (s32Ret != 0) { emxloge("AI_MaskClassification fail with %#x \n", s32Ret); }
    // }
    // if (m_faceDetection.ena) { FaceDetectionManage(); }
    // if (m_faceRecognition.ena) { FaceManage(&stFrame); }
    // VPSS_ReleaseChnFrame(m_vpssGrp, m_vpssChn, &stFrame);
}

void FaceAlgorithm::OnGetFrameComplete(void *arg) {
    //! 得到数据后这里才能push出去
    if (m_gotFaceRecognition) {
        MediaAiJob::PushAiData(&m_dataRecognition);
        m_dataRecognition.Free();
        m_gotFaceRecognition = false;
    }

    if (m_gotFaceDetection) {
        MediaAiJob::PushAiData(&m_dataFaceDetection);
        m_dataFaceDetection.Free();
        m_gotFaceDetection = false;
    }
}

ErrCodeE FaceAlgorithm::FaceRegister(const Json::Value &info, Json::Value &out) {
    std::string infoString;
    EasyJson::Serialize(info, infoString);
    emxlogi("info=%s\n", infoString.c_str());
    ErrCodeE e = ErrCodeE::Success;
    //人脸提取特征数据
    for (int i = 0; i < (int) info.size(); i++) {
        Json::Value faceInfo;
        std::string buf = "123456";
        faceInfo["id"] = info[i]["id"].asInt();
        //是使用jpg图片提取
        if (info[i]["isImg"].asBool()) {
            //e = getFeature(info[i]["path"].asString(),buf);
            emxlogi("register[%d],id=%d,path=%s\n", i, info[i]["id"].asInt(), info[i]["path"].asString().c_str());
        } else {
            //e = getSourceFeature(buf);
            emxlogi("register[%d],id=%d,path=null\n", i, info[i]["id"].asInt());
        }
        if (e == ErrCodeE::Success) {
            std::string feature;
            Base64::Encode(buf, feature);
            faceInfo["feature"] = feature;
            out.append(faceInfo);
        }
    }
    return e;
}

ErrCodeE FaceAlgorithm::FaceUnregister(const Json::Value &id) {

    std::string idString;
    EasyJson::Serialize(id, idString);
    emxlogi("id=%s\n", idString.c_str());
    for (int i = 0; i < (int) id.size(); i++) {
        int idNum = id[i].asInt();
        emxlogi("unregister[%d],id=%d\n", i, idNum);
        //删除对应id人脸库
        // faceDelete(idNum);
    }
    return ErrCodeE::Success;
}

ErrCodeE FaceAlgorithm::FaceSyncLibrary(const Json::Value &library) {
    std::string infoString;
    EasyJson::Serialize(library, infoString);
    emxlogi("library=%s\n", infoString.c_str());
    //同步特征数据
    for (int i = 0; i < (int) library.size(); i++) {
        int idNum = library[i]["id"].asInt();
        std::string feature = library[i]["feature"].asString();
        //添加人脸库
        std::string buf;
        Base64::Decode(feature, buf);
        emxlogi("lirary[%d] idNum=%d,feature dec=%s\n", i, idNum, buf.c_str());
        // addLibrary(idNum,buf);
    }
    return ErrCodeE::Success;
}

ErrCodeE FaceAlgorithm::Ctrl(const Json::Value &in, Json::Value &out) {
    ErrCodeE e = ErrCodeE::OperationNotSupport;
    if (in.isMember("faceRegister")) { e = FaceRegister(in["faceRegister"], out); }
    if (in.isMember("faceUnregister")) { e = FaceUnregister(in["faceUnregister"]); }
    if (in.isMember("syncLibrary")) { e = FaceSyncLibrary(in["syncLibrary"]); }
    return e;
}