//
// Created by xiong on 2022/4/27.
//

#ifndef EMX_MediaAi_HPP
#define EMX_MediaAi_HPP

#include "media/client/MediaParamCBase.hpp"
#include "media/common/MediaInfAi.hpp"
#include "media/common/MediaInfAiData.hpp"
#include "media/common/MediaInfAiDataFaceSnap.hpp"
#include "media/common/MediaInfAiDataMotionDetection.hpp"
#include "media/common/MediaInfAiDataPersonDetection.hpp"
#include "media/common/MediaRingBuffer.hpp"

namespace Emx {
    //!@brief AI
    class MediaAi : public MediaInfAi, public MediaParamCBase<Json::Value> {
    public:
        explicit MediaAi(int32_t chn) : MediaParamCBase(MediaModE::Ai, chn) {}

        ErrCodeE SetParam(const Json::Value &param) override;

        ErrCodeE GetParam(Json::Value &param) override;

        ErrCodeE GetVersion(std::string &version) override;
    };

    //!@brief AI数据接收基类
    class MediaAiDataRecvBase {
    public:
        MediaAiDataRecvBase() {}

        virtual ~MediaAiDataRecvBase() {}

        /*!
         * @brief 仅接收到AI消息头信息的回调函数(未去接收实际消息数据)
         * @param header [in] 消息头信息
         */
        using OnGetInfoCallback = std::function<void(MediaInfAiData::Header &header)>;

        /**
         * @brief 通过OnGetInfoCallback获取到header之后可通过此函数接收实际的消息数据
         * @param data [in,out] AI消息头信息
         * @return ErrCodeE
         */
        ErrCodeE GetDataByInfo(const MediaInfAiData::Header &header, MediaInfAiData *data);

    protected:
        void OnMsgRecv(uint32_t topic, const char *data, int32_t size);

    protected:
        OnGetInfoCallback m_cbInfo;
        MediaRingBufferReader m_ring;

        ErrCodeE OnGetBuffer(ErrCodeE e, const uint8_t *buffer, void *usr, int32_t off, int32_t size, uint32_t magicId);
    };

    //!@brief 异步AI数据接收
    class MediaAiDataRecvAsync : public MediaAiDataRecvBase {
    public:
        MediaAiDataRecvAsync() : MediaAiDataRecvBase() {}

        virtual ~MediaAiDataRecvAsync() {}

        /**
         * @brief 开始接收AI消息数据
         * @param cb [in] 获取到数据后的回调函数,如不需要接收实际数据，可填nullptr
         * @param cbInfo [in] 仅获取到消息头信息的回调函数(可选)
         * @return ErrCodeE
         */
        ErrCodeE Start(EuvLoop *loop, char *buffer, int size, OnGetInfoCallback cbInfo);

        //!@brief 停止接收AI消息数据
        void Stop();

        //!@brief 暂停接收AI消息数据
        void RecvFramePause() { m_sub.PauseRecv(); }

        //!@brief 恢复接收AI消息数据
        void RecvFrameResume() { m_sub.ResumeRecv(); }

        //!@brief 是否正在接收AI消息数据
        bool IsReceivingFrame() { return m_sub.IsReceiving(); }

    private:
        EuvLoop *m_loop;
        MsgTopicSubscriberAsync m_sub;
    };

    //!@brief 同步AI数据接收
    class MediaAiDataRecvSync : public MediaAiDataRecvBase {
    public:
        MediaAiDataRecvSync() : MediaAiDataRecvBase() {}

        /**
         * @brief 打开接收端口
         * @param timeoutMs [in] 打开操作超时时间
         * @return ErrCodeE
         */
        ErrCodeE Open(int32_t timeoutMs);

        //!@brief 关闭接收端口
        void Close();

        /**
         * @brief 获取消息头信息
         * @param data [out] 获取到的消息头
         * @param timeoutMs [in] 超时时间
         * @return ErrCodeE
         */
        ErrCodeE GetInfo(MediaInfAiData::Header &header, int32_t timeoutMs);

    private:
        MsgTopicSubscriberSync m_sub;
    };

}// namespace Emx
#endif//EMX_MediaAi_HPP
