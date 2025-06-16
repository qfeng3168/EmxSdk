//
// Created by tfh on 2024/09/02.
//

#ifndef EMX_MediaAov_HPP
#define EMX_MediaAov_HPP

#include "media/common/MediaInfAov.hpp"
#include "MediaParamCBase.hpp"
#include "EmxCore.hpp"

namespace Emx {
    //!@brief 视频输出
    class MediaAov : public MediaInfAov, public MediaParamCBase<MediaInfAov::Param> {
    public:
        explicit MediaAov(int32_t chn) : MediaParamCBase(MediaModE::Aov, chn) {}

        /**
         * @brief 更新aov状态
         * @param type [in] 状态类型
         * @return ErrCodeE
         */
        ErrCodeE UpdateStatus(AovStatusTypeE type) override;
    };
    //!@brief 异步Aov消息数据接收
    class MediaAovMessage {
    public:
        MediaAovMessage() : m_started(false) {}

        //!@brief 异步Aov消息接收回调函数
        using OnGetInfoCallback = std::function<void(MediaInfAov::AovPushFrame &frame)>;

        /**
         * @brief 开始接收Aov消息数据
         * @param loop [in] loop
         * @param buffer [in] 提供一个buffer
         * @param bufferSize [in] 提供的buffer大小
         * @param cb [in] 获取到数据后的回调函数
         * @return ErrCodeE
         */
        ErrCodeE Start(EuvLoop &loop, char *buffer, int bufferSize, OnGetInfoCallback cb);

        //!@brief 停止接收ISP消息数据
        void Stop();

    protected:
        void OnMsgRecv(uint32_t topic, const char *data, int32_t size);

    private:
        MsgTopicSubscriberAsync m_sub;
        OnGetInfoCallback m_cb;
        bool m_started;
    };
}// namespace Emx

#endif//EMX_MediaAov_HPP
