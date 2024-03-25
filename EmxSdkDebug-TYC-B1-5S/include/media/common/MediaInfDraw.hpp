//
// Created by tangfahong on 2024/3/21.
//

#ifndef EMX_MediaInfDraw_HPP
#define EMX_MediaInfDraw_HPP

#include "media/common/MediaTypedef.hpp"
#include <vector>

namespace Emx {
    class MediaInfDraw {
    public:
        static const int RateBase = 10000; //!<偏移量比率基准
       
        //!@brief 坐标点
        struct Point {
            float x;//!<横坐标  基准值RateBase
            float y;//!<纵坐标  基准值RateBase
        };
        //!@brief 线段
        struct Line {
            Point sp;      //!<起点
            Point ep;      //!<终点
            int thickness; //!<线宽
            uint32_t color;//!<颜色 rgba ff0000ff(默认红色)
        };

        //!@brief 线段持久化参数
        struct ParamItem {
            bool ena; //!<是否使能
            int layer;//!<显示层级
            std::vector<Line> line;
        };

        struct Param {
            std::vector<std::shared_ptr<ParamItem>> array;
        };
    };
}// namespace Emx

#endif//EMX_MediaInfDraw_HPP
