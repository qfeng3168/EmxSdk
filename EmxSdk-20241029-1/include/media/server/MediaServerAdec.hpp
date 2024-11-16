//
// Created by xiong on 2020/10/27.
//

#ifndef EMX_MediaServerAdec_HPP
#define EMX_MediaServerAdec_HPP

#include "EmxCore.hpp"
#include "MediaServerBase.hpp"
#include "media/common/MediaInfAdec.hpp"
#include <queue>

namespace Emx {
    class AudioDecoder;

    class MediaServerAdecChn : public MediaInfAdec, public MediaServerModuleChnBase {
    public:
        explicit MediaServerAdecChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Adec, chn) {}

        ~MediaServerAdecChn() override = default;

        ErrCodeE LoadParam() override;

        ErrCodeE SaveParam() override;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }

        void *GetRunningParam() override { return &m_paramRunning; }

        int GetParamSize() override { return sizeof(Param); }

        ErrCodeE GetParamDefaultData(std::string &data) override;

        bool ParamValid() override;

        ErrCodeE OnRecvReqChn(uint32_t method, const char *data, int32_t size, std::string &out) override;

        ErrCodeE SetVolumeTemp(int32_t volume) override { return ErrCodeE::Success; }

        ErrCodeE OnServerCreate() override;

        void OnServerDestroy() override;

    protected:
        /**
         * @brief 发送PCM数据到音频输出
         * @param pcm [in] 16bit pcm原始数据指针
         * @param pktNum [in] pcm采样数，注意不是数据字节数
         */
        virtual void SendToAo(const int16_t *pcm, int32_t pktNum) = 0;

        /**
         * @brief 清空Ao的缓存buffer
         */
        virtual void ClearAoBuffer() {}

        /**
         * @brief 填充静音数据
         */
        virtual void PadSilentPcm();

    protected:
        void OnGetMsg(uint32_t topic, const char *data, int32_t size);

        void RunPlayWork();

        void PlayFile(void *arg);

        void PlayFileDone(ErrCodeE e, void *arg);

        void SpkEna(bool ena);
        /**
         * @brief 发送PCM数据到芯片音频输出
         * @param pcm [in] 16bit pcm原始数据指针
         * @param pktNum [in] pcm采样数，注意不是数据字节数
         */
        void SendToChipAo(const int16_t *pcm, int32_t pktNum);

    protected:
        Param m_param;
        Param m_paramNew;
        Param m_paramRunning;
        uint32_t m_silentTimeMs = 500;  //!< 推送静音数据时间 ms
        uint32_t m_delayPlayMs = 0;//!< 打开功放后延时播放音频时间 ms
    private:
        EuvLoop m_loopAdec;
        MsgTopicSubscriberAsync m_sub;
        AudioDecoder *m_decoder;
        char m_buffer[EMX_AUDIO_MAX_FRAME_SIZE * 2];
        struct VoiceFile {
            std::string path;
            int volume;
        };
        std::queue<VoiceFile> m_voiceQue;
        EuvWork m_playWork;
        bool m_playQuit;
        bool m_autoMute;
        std::list<Gpio *> m_spkGpioList;
        EuvTimer m_muteTimer;
        int m_muteCnt;
        bool m_muting;
        int m_playCnt;
    };

    class MediaServerAdec : public MediaServerModuleBase {
    public:
        MediaServerAdec() : MediaServerModuleBase(MediaModE::Adec, "adec") {}
    };
}// namespace Emx

#endif//EMX_MediaServerAdec_HPP
