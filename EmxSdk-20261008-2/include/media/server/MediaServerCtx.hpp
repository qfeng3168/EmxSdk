//
// Created by xiong on 2023/8/21.
//

#ifndef EMX_MediaServerCtx_HPP
#define EMX_MediaServerCtx_HPP

#include "media/common/MediaRingBuffer.hpp"
#include <mutex>

namespace Emx {
    /** @addtogroup media-server
     *  @{
     */
    class MediaFactory;

    class MediaServerCtx {
    public:
        static MediaServerCtx *GetInst() { return &m_inst; }

        static bool CheckRangeLegal(const char *name, int param, Json::Value &json);

        static bool CheckItemLegal(const char *name, int param, Json::Value &json);

        ErrCodeE SetParam(const char *name, const Json::Value &json);

        ErrCodeE GetParam(const char *name, Json::Value &json);

        ErrCodeE GetParamDefault(const char *name, Json::Value &json);

        ErrCodeE GetRange(const char *name, Json::Value &json);

        ErrCodeE GetCfg(const char *name, Json::Value &json);

        ErrCodeE SetScene(const char *scene);

        ErrCodeE DirectControl(Json::Value &cmd, Json::Value &out);

        const char *GetScene() { return m_scene; }

    public:
        MediaFactory *factory;
        EuvLoop loop;
        MsgTopicPublisherAsync pub;
        char buffer[4 * 1024];
        MediaRingBufferWriter ring;
        bool mediaUtcOutput;//!< 是否输出媒体utc时间戳
        //        MediaRingBufferWriter ringAi;
        //        std::mutex lockAi;
    private:
        MediaServerCtx() {}

        static MediaServerCtx m_inst;
        char m_scene[EMX_MAX_PATH_SIZE];
    };

    /** @}*/
}// namespace Emx

#endif//EMX_MediaServerCtx_HPP
