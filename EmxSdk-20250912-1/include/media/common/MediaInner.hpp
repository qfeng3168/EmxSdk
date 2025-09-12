//
// Created by xiong on 2021/7/30.
//

#ifndef EMX_MediaInner_HPP
#define EMX_MediaInner_HPP

#define MEDIA_RING_BUFFER_KEY 0x12345000
#define MEDIA_RING_VO_BUFFER_KEY 0x12346000

//MediaServer Msg Client ID
#define MEDIA_SERVER_MSG_ID 0x00020000

#define MEDIA_TOPIC_SERVER (MEDIA_SERVER_MSG_ID + 0x100)

#define MEDIA_TOPIC_SYSTEM (MEDIA_SERVER_MSG_ID + 0x200)

//Isp
#define MEDIA_TOPIC_ISP (MEDIA_SERVER_MSG_ID + 0x300)
#define MEDIA_TOPIC_ISP_INFO (MEDIA_TOPIC_ISP + 0x01)
#define MEDIA_TOPIC_ISP_FPS_CHANGE (MEDIA_TOPIC_ISP + 0x02)

//Venc
#define MEDIA_TOPIC_VENC (MEDIA_SERVER_MSG_ID + 0x400)
#define MEDIA_TOPIC_VENC_DRAW_RECT (MEDIA_TOPIC_VENC + 0x20)
#define MEDIA_TOPIC_VENC_FRAME (MEDIA_TOPIC_VENC + 0x40)

//Aenc
#define MEDIA_TOPIC_AENC (MEDIA_SERVER_MSG_ID + 0x500)
#define MEDIA_TOPIC_AENC_AENC (MEDIA_TOPIC_AENC + 0x20)
#define MEDIA_TOPIC_AENC_PCM (MEDIA_TOPIC_AENC + 0x40)

//Adec
#define MEDIA_TOPIC_ADEC (MEDIA_SERVER_MSG_ID + 0x600)
#define MEDIA_TOPIC_ADEC_FRAME (MEDIA_TOPIC_ADEC + 0x20)
#define MEDIA_TOPIC_ADEC_RAW_PCM (MEDIA_TOPIC_ADEC + 0x40)
#define MEDIA_TOPIC_ADEC_VOICE_PALY (MEDIA_TOPIC_ADEC + 0x60)
#define MEDIA_TOPIC_ADEC_PALY_CTRL (MEDIA_TOPIC_ADEC + 0x80)
//snap
#define MEDIA_TOPIC_SNAP (MEDIA_SERVER_MSG_ID + 0x700)
//qr scan
#define MEDIA_TOPIC_QR_SCAN (MEDIA_SERVER_MSG_ID + 0x900)
//ai
#define MEDIA_TOPIC_AI (MEDIA_SERVER_MSG_ID + 0xa00)
//vdev
#define MEDIA_TOPIC_VDEC (MEDIA_SERVER_MSG_ID + 0xb00)
#define MEDIA_TOPIC_VDEC_FRAME (MEDIA_TOPIC_VDEC + 0x20)
#define MEDIA_TOPIC_VDEC_RAW_FRAME (MEDIA_TOPIC_VDEC + 0x40)
//aov
#define MEDIA_TOPIC_AOV (MEDIA_SERVER_MSG_ID + 0xc00)
#define MEDIA_TOPIC_AOV_PUSH (MEDIA_TOPIC_AOV + 0x10)

//draw
#define MEDIA_TOPIC_DRAW (MEDIA_SERVER_MSG_ID + 0xd00)
#define MEDIA_TOPIC_DRAW_PUSH_USER_PARAM (MEDIA_TOPIC_DRAW + 0x10)
#define MEDIA_TOPIC_DRAW_CLEAR_USER_PARAM (MEDIA_TOPIC_DRAW + 0x20)
#define MEDIA_TOPIC_DRAW_PUSH_ALL_PARAM (MEDIA_TOPIC_DRAW + 0x30)
#define MEDIA_TOPIC_DRAW_CLEAR_ALL_PARAM (MEDIA_TOPIC_DRAW + 0x40)

namespace Emx {
    /** @addtogroup media-common
     *  @{
     */
    ////////////////RPC///////////////////
    //MediaServerMethod
    enum class MediaServerMethodE : uint32_t {
        Create = 1,
        Destroy,
        SwitchScene,
        SwitchSceneTemp,
        GetScene,
        Butt,
    };

    //MediaParamMethod
    enum class MediaParamMethodE : uint32_t {
        SetParam = 1,
        GetParam,
        GetParamRange,
        GetChnNum,
        GetParamDefault,
        SetTmpParam,
        GetRunningParam,
        Butt,
    };
    enum class MediaSystemMethodE : uint32_t {
        GetChipInfo = (uint32_t) MediaParamMethodE::Butt,//!<获取芯片信息
    };
    enum class MediaIspMethodE : uint32_t {
        SetRunMode = (uint32_t) MediaParamMethodE::Butt,
        GetRunMode,
        SetGrey,
    };

    enum class MediaQrScanMethodE : uint32_t {
        Start = (uint32_t) MediaParamMethodE::Butt,
        Stop,
    };

    enum class MediaAencMethodE : uint32_t {
        SetVolume = (uint32_t) MediaParamMethodE::Butt,
        Mute,
        OnOffCtrl,
    };

    enum class MediaAdecMethodE : uint32_t {
        SetVolume = (uint32_t) MediaParamMethodE::Butt,
        SetVolumeTemp,
        GetPlayStatus,
    };

    enum class MediaSnapMethodE : uint32_t {
        Trigger = (uint32_t) MediaParamMethodE::Butt,
    };

    enum class MediaVencMethodE : uint32_t {
        FlushIdr = (uint32_t) MediaParamMethodE::Butt,
    };

    enum class MediaViMethodE : uint32_t {
        SetFps = (uint32_t) MediaParamMethodE::Butt,
        SetRotate,
    };

    enum class MediaDoubleLensMethodE : uint32_t {
        SwitchMode = (uint32_t) MediaServerMethodE::Butt,
        SetOutMode,
        GetOutMode,
        DeleteMode,
    };

    enum class MediaVdecMethodE : uint32_t {
        Create = (uint32_t) MediaParamMethodE::Butt,
        Destroy,
    };

    enum class MediaAiMethodE : uint32_t {
        GetVersion = (uint32_t) MediaParamMethodE::Butt,
        Ctrl,
    };

    enum class MediaAovMethodE : uint32_t {
        UpdateStatus = (uint32_t) MediaParamMethodE::Butt,
    };

    enum class MediaOsdMethodE : uint32_t { SetOsdContent = (uint32_t) MediaParamMethodE::Butt };
    /** @}*/
}// namespace Emx

#endif//EMX_MediaInner_HPP
