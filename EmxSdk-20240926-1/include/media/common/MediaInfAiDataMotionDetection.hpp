//
// Created by xiong on 2023/9/12.
//

#ifndef EMX_MediaInfAiMotionDetection_HPP
#define EMX_MediaInfAiMotionDetection_HPP

#include "MediaInfAiData.hpp"
#include "MediaRegion.hpp"
#include <cstring>

namespace Emx {

    class MediaInfAiDataMotionDetection : public MediaInfAiData {
    public:
        MediaInfAiDataMotionDetection() : MediaInfAiData(), num(0), array(nullptr) {}

        virtual ~MediaInfAiDataMotionDetection() { Free(); }

    public:
        void Alloc(int n) {
            Free();
            array = (Rect *) EP_MALLOC(n * (int) sizeof(Rect));
            if (array) { memset((void *) array, 0, n * sizeof(Rect)); }
        }

        void Free() {
            if (array) {
                free(array);
                array = nullptr;
            }
        }

        int GetSerializeSize() override { return (int) sizeof(num) + num * (int) sizeof(Rect); }

        void Serialize(uint8_t *buffer) override {
            int off = 0;
            memcpy(buffer + off, &num, sizeof(num));
            off += sizeof(num);
            memcpy(buffer + off, array, num * (int) sizeof(Rect));
        }

        void Parse(const uint8_t *buffer, int size) override {
            int off = 0;
            memcpy(&num, buffer + off, sizeof(num));
            off += sizeof(num);
            if (num && num * (int) sizeof(Rect) + off == size) {
                Alloc(num);
                memcpy(array, buffer + off, num * (int) sizeof(Rect));
            }
        }

    public:
        int num;
        Rect *array;
    };
}// namespace Emx

#endif//EMX_MediaInfAiMotionDetection_HPP