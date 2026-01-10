//
// Created by xiong on 2021/4/23.
//

#ifndef EMX_MediaIsp_HPP
#define EMX_MediaIsp_HPP

#include "media/common/MediaInfIsp.hpp"
#include "MediaParamCBase.hpp"
#include "EmxCore.hpp"

namespace Emx {
    /** @addtogroup media-client
     *  @{
     */
    //!@brief Isp
    class MediaIsp : public MediaInfIsp, public MediaParamCBase<MediaInfIsp::Param> {
    public:
        explicit MediaIsp(int32_t chn) : MediaParamCBase(MediaModE::Isp, chn) {}

        ErrCodeE SetRunMode(RunModeE mode) override;

        ErrCodeE GetRunMode(RunModeE &mode) override;
        
        ErrCodeE GetIspInfo(MediaIsp::Info &info) override;
    };

    //!@brief 异步ISPInfo数据接收
    class MediaIspInfo {
    public:
        MediaIspInfo() : m_started(false) {}


        //!@brief 异步ISP信息接收回调函数
        using OnGetInfoCallback = std::function<void(MediaIsp::Info &info)>;

        /**
         * @brief 开始接收ISP消息数据
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
    /** @}*/
}
#endif //EMX_MediaIsp_HPP

