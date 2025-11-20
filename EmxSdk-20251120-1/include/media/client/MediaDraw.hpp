/*
* @Author: tangfahong
* @Date: 2024/3/22
*/

#ifndef EMX_MediaDraw_HPP
#define EMX_MediaDraw_HPP

#include "MediaParamCBase.hpp"
#include "media/common/MediaInfDraw.hpp"
#include "core/EmxUtils.hpp"
#include <cstring>
#include <list>

namespace Emx {
    /** @addtogroup media-client
     *  @{
     */
    //!@brief Draw
    class MediaDraw : public MediaInfDraw, public MediaParamCBase<MediaInfDraw::Param> {
    public:
        explicit MediaDraw(int32_t chn) : MediaParamCBase(MediaModE::Draw, chn) {}

        ErrCodeE SetParam(const Param &param) override;

        ErrCodeE GetParam(Param &param) override;

        ErrCodeE GetParamDefault(Param &param) override;

        ErrCodeE SetTmpParam(const Param &param) override;

        ErrCodeE GetRunningParam(Param &param) override;
    };

    //!@brief Draw 动态配置异步接口,均为异步操作
    class MediaDrawAsync{
    public:
        /**
         * @brief 创建异步Draw动态配置流。
         * 
         * 该方法绑定到指定的事件循环，创建异步Draw动态配置流。
         * @param loop 要绑定的事件循环对象。
         * @param paramNumMax 配置的paramNum最大数量，缓存区buffer大小
         * @return 操作结果，成功返回 ErrCodeE::Success，失败返回相应错误码。
         */
        ErrCodeE Create(EuvLoop &loop, uint32_t paramNumMax);

        /**
         * @brief 发送用户的Draw动态配置显示，会叠加在已经配置的param上面
         * @param chn [in] 目的通道号
         * @param userParam [in] 用户的Draw动态配置
         * @return ErrCode
         */
        ErrCodeE PushUserParam(int32_t chn, const MediaInfDraw::Param &userParam);

        /**
         * @brief 清除用户的Draw动态配置的userParam,不会清除已经配置的param
         * @param chn [in] 目的通道号
         * @return ErrCode
         */
        ErrCodeE ClearUserParam(int32_t chn);

        /**
         * @brief 发送所有的Draw配置显示，会替换所有的param
         * @param chn [in] 目的通道号
         * @param param [in] Draw配置
         * @return ErrCode
         */
        ErrCodeE PushAllParam(int32_t chn, const MediaInfDraw::Param &param);

        /**
         * @brief 清除所有的Draw配置的param
         * @param chn [in] 目的通道号
         * @return ErrCode
         */
        ErrCodeE ClearAllParam(int32_t chn);

        /**
         * @brief 销毁异步Draw动态配置。
         */
        void Destroy();
    private:
        /**
         * @brief 发送Draw配置显示
         * @param chn [in] 目的通道号
         * @param isAll [in] 是否发送所有的Draw配置
         * @param param [in] Draw配置
         * @return ErrCode
         */
        ErrCodeE PushParam(int32_t chn,bool isAll, const MediaInfDraw::Param &param);

        /**
         * @brief 清除Draw配置的param
         * @param chn [in] 目的通道号
         * @param isAll [in] 是否清除所有的Draw配置
         * @return ErrCode
         */
        ErrCodeE ClearParam(int32_t chn,bool isAll);

    private:
        MsgTopicPublisherAsync m_pub;//!< 异步消息主题发布器
        int m_chn;                   //!< 解码通道号
        uint32_t m_paramNumMax;           //!< 最大配置数量
        uint8_t *m_buffer;           //!< 缓冲区
        uint32_t m_bufferSize;       //!< 缓冲区大小
        uint32_t m_userSize;         //!< 用户数据缓冲区大小
    };
    /** @}*/
}// namespace Emx
#endif//EMX_MediaOSD_HPP
