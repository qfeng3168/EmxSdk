//
// Created by tangfahong on 2026/06/22.
//

#ifndef EMX_MediaInfAiDataJsonData_HPP
#define EMX_MediaInfAiDataJsonData_HPP

#include "MediaInfAiData.hpp"
#include <cstring>

namespace Emx {
    /** @addtogroup media-common
     *  @{
     */

    class MediaInfAiDataJsonData : public MediaInfAiData {
    public:
        MediaInfAiDataJsonData() : MediaInfAiData(), msgSize(0) { msg.clear(); }

        virtual ~MediaInfAiDataJsonData() { Clear(); }

    public:
        //!@brief 清空Json数据
        void Clear() { msg.clear(); }

        int GetSerializeSize() override {
            msgSize = msg.toStyledString().length();
            return msgSize;
        }

        void Serialize(uint8_t *buffer) override {
            std::string data = msg.toStyledString();
            msgSize = static_cast<int>(data.length());
            memcpy(buffer, data.c_str(), msgSize);
        }

        void Parse(const uint8_t *buffer, int size) override { EasyJson::Parse((const char*)buffer, size, msg); }

    public:
        int msgSize;    //!<Json数据大小
        Json::Value msg;//!<Json数据
    };
    /** @}*/
}// namespace Emx

#endif//EMX_MediaInfAiDataJsonData_HPP