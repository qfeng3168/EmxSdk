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
        //!@brief DRAW类型
        enum class TypeE : uint8_t {
            Line = 0, //!<线条显示
            Rect, //!<矩形框显示
            Ellipse, //!<椭圆显示
        };
        //!@brief 坐标点
        struct Point {
            float x;//!<横坐标  基准值RateBase
            float y;//!<纵坐标  基准值RateBase
        };
        //!@brief 线段
        struct ParamLine {
            Point sp;      //!<起点
            Point ep;      //!<终点
        };

        //!@brief 矩形
        struct ParamRect {
            bool fill;  //!<是否是实心
            float x;  //!<起点x
            float y;  //!<起点y
            float width;  //!<矩形宽
            float height; //!<矩形高
        };
        //!@brief 椭圆
        struct ParamEllipse {
            bool fill;  //!<是否是实心
            float x;  //!<中心点x
            float y;  //!<中心点y
            float majorAxis;//!<长轴
            float minorAxis;//!<短轴
        };

        //!@brief 持久化参数
        struct ParamItem {
            TypeE type; //!<DRAW类型
            bool ena; //!<是否使能
            int layer;//!<显示层级
            int thickness; //!<线宽
            uint32_t color;//!<颜色 rgba ff0000ff(默认红色)
            union {
                ParamLine line; //!<画线类型，支持各种形状
                ParamRect rect; //!<矩形框类型，仅支持矩形
                ParamEllipse ellipse; //!<椭圆形框类型
            };
        };

        struct Param {
            std::vector<std::shared_ptr<ParamItem>> array;
        };
    };
}// namespace Emx

#endif//EMX_MediaInfDraw_HPP
