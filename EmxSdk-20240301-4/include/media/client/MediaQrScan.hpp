//
// Created by xiong on 2021/6/25.
//

#ifndef EMX_MediaQRScan_HPP
#define EMX_MediaQRScan_HPP

#include "media/common/MediaInfQrScan.hpp"
#include "MediaParamCBase.hpp"
#include "core/EmxMsg.hpp"
#include "core/EmxUV.hpp"

namespace Emx {
    //!@brief 二维码扫描
    class MediaQrScan : public MediaInfQrScan, public MediaParamCBase<MediaInfQrScan::Param> {
    public:
        explicit MediaQrScan(int32_t chn) : MediaParamCBase(MediaModE::QrScan, chn) {
            m_started = false;
        }

        /**
         * @brief 当二维码识别结束后触发此回调
         * @param chn [in] 通道号
         * @param result [in] 检测结果字符串
         */
        using OnGetResult = std::function<void(int chn, const char *result)>;

        /**
         * @brief 开始扫描二维码
         * @param loop [in] 需要绑定的loop
         * @param buffer [in] 提供扫码使用的loop buffer
         * @param size [in] buffer的大小
         * @param cbRes [in] 扫码成功后的回调函数
         * @return ErrCodeE
         */
        ErrCodeE StartQR(EuvLoop &loop, char *buffer, int size, OnGetResult cbRes);

        /**
         * @brief 结束扫码
         * @return ErrCodeE
         */
        ErrCodeE StopQR();

    private:
        ErrCodeE Start() override;

        ErrCodeE Stop() override;

    private:
        bool m_started;
        MsgTopicSubscriberAsync m_sub;
        OnGetResult m_cb;
    };
}

#endif //EMX_MediaQRScan_HPP

