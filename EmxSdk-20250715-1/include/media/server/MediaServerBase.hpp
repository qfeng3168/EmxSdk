//
// Created by xiong on 2023/8/16.
//

#ifndef EMX_MediaServerGlobalBase_HPP
#define EMX_MediaServerGlobalBase_HPP

#include "media/common/MediaTypedef.hpp"
#include "json/json.h"
#include <vector>

namespace Emx {
    /** @addtogroup media-server
     *  @{
     */
    class MediaServerModuleBase;

    class MediaServerModuleChnBase {
    public:
        explicit MediaServerModuleChnBase(MediaModE mod, int32_t chn);

        virtual ~MediaServerModuleChnBase() = default;

        /**
         * @brief 创建模块通道
         * @return ErrCodeE
         */
        virtual ErrCodeE OnServerCreate() { return OnCreate(); }

        /**
         * @brief 创建模块通道
         * @return ErrCodeE
         */
        virtual ErrCodeE OnCreate() = 0;

        /**
         * @brief 销毁模块通道
         */
        virtual void OnServerDestroy() { OnDestroy(); }

        /**
         * @brief 销毁模块通道
         */
        virtual void OnDestroy() = 0;


        /**
         * @brief 参数改变
         * @return ErrCodeE
         */
        virtual ErrCodeE OnParamChanged() = 0;

        virtual ErrCodeE LoadParam() { return ErrCodeE::Success; }

        virtual ErrCodeE SaveParam() { return ErrCodeE::Success; }

        virtual ErrCodeE GetParamRange(Json::Value &range);

        virtual void *GetParam() = 0;

        virtual void *GetNewParam() = 0;

        virtual void *GetRunningParam() = 0;

        virtual int GetParamSize() = 0;

        virtual ErrCodeE GetParamDefaultData(std::string &data) { return ErrCodeE::Success; }

        virtual void NewParamInCome(const char *data, int size) { memcpy(GetNewParam(), data, GetParamSize()); }

        virtual bool ParamValid() { return true; }
        
        virtual void GetRunningParamData(std::string &data) { data.assign((const char *) GetRunningParam(), GetParamSize()); }

        virtual void GetParamData(std::string &data) { data.assign((const char *) GetParam(), GetParamSize()); }

        virtual bool ParamChanged() { return memcmp(GetRunningParam(), GetNewParam(), GetParamSize()) != 0; }

        virtual void UpdateParam() { memcpy(GetParam(), GetNewParam(), GetParamSize()); }

        virtual void UpdateRunningParam() { memcpy(GetRunningParam(), GetNewParam(), GetParamSize()); }

        virtual void ReleaseNewParam() {}

        virtual ErrCodeE OnRecvReqChn(uint32_t method, const char *data, int32_t size, std::string &out) {
            return OnRecvReqChnSpec(method, data, size, out);
        }

        virtual ErrCodeE OnRecvReqChnSpec(uint32_t method, const char *data, int32_t size, std::string &out) {
            return ErrCodeE::OperationNotSupport;
        }

        int32_t GetChn() { return m_chn; }

    protected:
        MediaServerModuleBase *m_module;
        int32_t m_chn;
    };

    /**
     * @brief 媒体模块的通用基类
     * @tparam ParamT 模块的参数类型模板
     */
    class MediaServerModuleBase {
    public:
        MediaServerModuleBase(MediaModE mod, const char *name) : m_mod(mod), m_name(name) {}

        /**
         * @brief 模块构造函数，通过配置文件的通道数，创建模块的各个通道
         */
        virtual void Create();

        /**
         * @brief 模块析构函数，销毁各个通道
         */
        virtual void Destroy();

        virtual ErrCodeE OnRecvReq(uint32_t method, int32_t chn, const char *data, int32_t size, std::string &out);

        virtual int LoadChnNum();

        MediaModE m_mod;
        std::string m_name;
        std::vector<MediaServerModuleChnBase *> m_array;
    };

    /** @}*/
}// namespace Emx

#endif//EMX_MediaServerGlobalBase_HPP
