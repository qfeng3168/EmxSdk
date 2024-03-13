//
// Created by xiong on 2023/8/16.
//

#ifndef EMX_MediaParamCBase_HPP
#define EMX_MediaParamCBase_HPP

#include "media/common/MediaTypedef.hpp"
#include "json/json.h"


namespace Emx {
    class MediaParamCBaseImpl {
    public:
        static ErrCodeE SetParam(uint32_t model, int32_t chn, const void *param, int32_t size, int32_t timeoutMs);

        static ErrCodeE GetParam(uint32_t model, int32_t chn, void *param, int32_t size, int32_t timeoutMs);

        static ErrCodeE GetParamRange(uint32_t model, int32_t chn, Json::Value &range, int32_t timeoutMs);

        static ErrCodeE GetTotalChnNum(uint32_t model, int32_t &num, int32_t timeoutMs);
    };

    //!@brief 客户端参数基类，方便通用操作
    template<class Param>
    class MediaParamCBase{
    public:
        explicit MediaParamCBase(MediaModE model, int32_t chn) : m_modelId((uint32_t) model), m_chn(chn) {}

        virtual ~MediaParamCBase() = default;

        /**
         * @brief 设置参数
         * @param param [in] 参数数据
         * @return ErrCodeE
         */
        virtual ErrCodeE SetParam(const Param &param) {
            return MediaParamCBaseImpl::SetParam(m_modelId, m_chn, &param, sizeof(param), 2000);
        }

        /**
         * @brief 获取参数
         * @param param [out] 参数数据
         * @return ErrCodeE
         */
        virtual ErrCodeE GetParam(Param &param) {
            return MediaParamCBaseImpl::GetParam(m_modelId, m_chn, &param, sizeof(param), 2000);
        }

        /**
         * @brief 获取参数取值范围
         * @param range [out] 参数取值范围
         * @return ErrCodeE
         */
        virtual ErrCodeE GetParamRange(Json::Value &range)  {
            return MediaParamCBaseImpl::GetParamRange(m_modelId, m_chn, range, 2000);
        }

        /**
         * @brief 获取支持的总通道数量
         * @param num [out] 通道数量
         * @return ErrCode
         */
        virtual ErrCodeE GetTotalChnNum(int &num)  {
            return MediaParamCBaseImpl::GetTotalChnNum(m_modelId, num, 2000);
        }

        /**
         * @brief 获取通道号
         * @return 通道号
         */
        int32_t GetChn() { return m_chn; }

    protected:
        uint32_t m_modelId;
        int32_t m_chn;
    };
}
#endif //EMX_MediaParamCBase_HPP
