//
// Created by xiong on 2020/10/27.
//

#ifndef EMX_MEDIASERVER_HPP
#define EMX_MEDIASERVER_HPP

#include "media/common/MediaTypedef.hpp"
#include "media/server/MediaServerBase.hpp"
#include "EmxCore.hpp"
namespace Emx {
    class MediaServerCtx;

    class MediaFactory;

    class MediaServer {
    public:
        MediaServer();

        virtual ~MediaServer() {}

        using OnCreated = std::function<void()>;

        void Create(OnCreated cb, MediaFactory *factory);

        void Destroy();

    private:
        ErrCodeE OnRecvReq(uint32_t module, uint32_t method, int32_t chn,
                           const char *data, int32_t size, std::string &out);
    protected:
        virtual ErrCodeE OnRecvReqSub(uint32_t module, uint32_t method, int chn,
                                      const char *data, int32_t size, std::string &out) {
            return ErrCodeE::OperationNotSupport;
        }

    private:
        void RunRpc();

        void RunTopic();

        void Launcher();

    private:
        MediaServerCtx *m_ctx;
        MsgRpcRep m_resp;
        bool m_rpcConnected;
        bool m_topicConnected;
        OnCreated m_cb;
    };
}// namespace Emx

#endif//EMX_MEDIASERVER_HPP
