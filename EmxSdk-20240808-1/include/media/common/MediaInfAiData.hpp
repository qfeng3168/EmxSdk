//
// Created by xiong on 2023/9/12.
//

#ifndef EMX_MediaInfAiData_HPP
#define EMX_MediaInfAiData_HPP

#include "media/common/MediaRegion.hpp"
#include "media/common/MediaTypedef.hpp"
namespace Emx {

    struct MediaInfAiPic {
        enum class TypeE : uint8_t {
            Jpg = 0,
            Bmp,
            Png,
        } type;
        int32_t width;  /*图片宽*/
        int32_t height; /*图片高*/
        int32_t size;   /*图片大小*/
        uint8_t *img;   /*图片数据缓存指针*/
    };

    class MediaInfAiData {
    public:
        MediaInfAiData() : header({}) {}

        virtual ~MediaInfAiData() {}

        virtual int GetSerializeSize() = 0;

        virtual void Serialize(uint8_t *buffer) = 0;

        virtual void Parse(const uint8_t *buffer, int size) = 0;

    public:
        //!@brief AI数据类型
        enum class TypeE : uint8_t {
            MotionDetection = 0,      //!<移动侦测数据
            PersonDetection,          //!<人形侦测数据
            FaceDetection,            //!<人脸检测数据
            Classify,                 //!<分类数据
            FaceRecognition,          //!<人脸识别数据
            PersonRecognition,        //!<人形识别数据
            TypeNum,                  //!<数据类型数量
            PassengerFlowStatistics,  //!<客流统计数据
            CrossingDetection,        //!<越界数据
            InvasionDetection,        //!<区域入侵数据
            PersonVehicleNonDetection,//!<人车非检测
            ElectricBicycleDetection, //!<电动车检测
            VehicleRecognition,       //!<车辆识别含车牌
            RegionalPeopleStatistics, //!<区域人数统计
            OffDutyDetection,         //!<离岗检测
            BabyCryDetection,         //!<婴儿啼哭检测
        };
        //!@brief AI数据头
        struct Header {
            int32_t chn;         //!<通道号
            TypeE type;          //!<Ai数据类型
            uint64_t timestampMs;//!<时间戳
            Size srcPicSize;     //!<AI信息里坐标或长宽信息基于的原始图像大小
            int32_t size;        //!<信息数据大小
            uint8_t *data;       //!<信息数据内存指针
            int32_t off;         //!<ringbuffer偏移，用户不需要关心
        } header;
    };
}// namespace Emx

#endif//EMX_MediaInfAiData_HPP