//
// Created by tangfahong on 2024/3/1.
//

#ifndef EMX_MediaInfAiDataPassengerFlowStatistics_HPP
#define EMX_MediaInfAiDataPassengerFlowStatistics_HPP

#include "MediaInfAiDataMotionDetection.hpp"
#include "MediaRegion.hpp"
#include <cstring>

namespace Emx {

    class MediaInfAiDataPassengerFlowStatistics : public MediaInfAiDataMotionDetection {
    public:
        MediaInfAiDataPassengerFlowStatistics() : MediaInfAiDataMotionDetection(), entryNum(0), leaveNum(0) {}

    public:
        uint32_t entryNum; //!<进入人数
        uint32_t leaveNum; //!<离开人数
    };
}

#endif //EMX_MediaInfAiDataPassengerFlowStatistics_HPP