//
// Created by xiong on 2022/3/2.
//

#ifndef EMX_EmxParam_HPP
#define EMX_EmxParam_HPP

#include <json/json.h>
#include <functional>
#include "core/EmxTypeDef.hpp"
#include "core/EmxMsg.hpp"

namespace Emx {
    //!@brief 关于系统需要持久化保存的参数的相关操作
    class Param {
    public:
        /**
         * @brief 构造函数
         * @param [in] name 指定参数的名称(参数实际存储的文件名去掉后缀.json)
         */
        Param(const char *name);

        /**
         * @brief 构造函数
         * @param [in] name 指定参数的名称(参数实际存储的文件名去掉后缀.json)
         */
        Param(const std::string &name);

        /**
         * @brief 保存参数
         * @param [in] json 参数的json对象
         * @return ErrCode
         */
        ErrCodeE Set(const Json::Value &json);

        /**
         * @brief 获取参数
         * @param [out] json 参数的json对象
         * @return ErrCode
         */
        ErrCodeE Get(Json::Value &json);

        /**
         * @brief 获取默认参数
         * @param [out] json 参数的json对象
         * @return ErrCode
         */
        ErrCodeE GetDefault(Json::Value &json);

        /**
         * @brief 将存储的参数恢复为默认值
         * @return ErrCode
         */
        ErrCodeE Reset();

        /**
         * @brief 将设备的所有参数恢复为默认值(除resetReserved.json中声明保留的文件)
         * @return ErrCode
         */
        static ErrCodeE ResetAllToFactory();

    private:
        char m_name[EMX_MAX_PATH_SIZE];
    };

    //!用于监听参数设置事件
    class ParamSetEvent {
    public:
        struct ParamMsg{
            std::string name;//配置文件名称
            std::string data;//配置文件内容
        };

        /**
         * @brief 消息回调
         * @param msg [in] 信息
         * @return ErrCodeE
         */
        using OnParamMsgLinkCallback = std::function<void(ParamMsg &msg)>;
    public:

        ParamSetEvent() {
            m_inited = false;
        }

        //!@brief 连接到CoreServer时的回调
        using OnConnected = std::function<void()>;

        /**
         * @brief 创建监听
         * @param loop [in] 需要绑定的loop
         * @param buffer [in] 提供用于缓存的buffer
         * @param size [in] 用于缓存buffer的大小
         * @param cbLink [in] 当连接状态发生改变时的回调函数
         * @param cbConnected [in] 当此对象连接到CoreServer后的回调函数
         */
        void Create(EuvLoop &loop, char *buffer, int size,
                    OnParamMsgLinkCallback cbLink, OnConnected cbConnected = nullptr);

        //!@brief 销毁监听
        void Destroy();

    private:
        void OnMsgRecv(uint32_t topic, const char *data, int32_t size);

    private:
        bool m_inited;
        OnParamMsgLinkCallback m_cbLink;
        MsgTopicSubscriberAsync m_sub;
    };
}
#endif //EMX_EmxParam_HPP

