//
// Created by xiong on 2023/8/22.
//

#ifndef EMX_MediaInfIsp_HPP
#define EMX_MediaInfIsp_HPP

#include "media/common/MediaTypedef.hpp"

namespace Emx {

    class MediaInfIsp {
    public:
        //!@brief ISP运行模式
        enum class RunModeE : uint8_t {
            Day, //!<白天模式
            Night, //!<夜晚模式
            NightWithLight, //!<白光夜视模式
            User,  //!<用户自定义模式，应对特殊需求;
                    //!<如果有多种特殊场景模式，可以将此定义的文件A.bin做成软连接到可读写分区B.bin;
                    //!<切换场景时就替换可读写分区的B.bin,然后在下发模式;
            UnDef //!<未定义
        };

        //!@brief 操作模式
        enum class OptModeE : uint8_t {
            Auto, //!<自动模式
            Manual //!<手动模式
        };

        //!@brief 曝光增益
        enum class ExpGainE : uint8_t {
            X1 = 1,
            X2 = 2,
            X4 = 4,
            X8 = 8,
            X16 = 16,
            X32 = 32,
            X64 = 64,
        };

        //!@brief ISP持久化参数
        struct Param {
            OptModeE expMode; //!<曝光模式
            ExpGainE autoMaxExpGain; //!<最大自动曝光增益
            uint8_t compensation; //!<曝光补偿
            ExpGainE manualExpGain; //!<手动曝光增益
            int32_t expTime; //!<曝光时间
            uint8_t brightness; //!<亮度
            uint8_t sharpness; //!<锐度
            uint8_t hue; //!<色度
            uint8_t contrast; //!<对比度
            uint8_t saturation; //!<饱和度
            uint8_t deNoise; //!<降噪程度
            OptModeE wbMode; //!<白平衡模式
            uint8_t redGain; //!<手动白平衡下的红色增益
            uint8_t blueGain; //!<手动白平衡下的蓝色增益
            uint8_t antiFlickerHz; //!<抗闪烁频率
            bool forceAntiFlickerEna; //!<强制抗闪烁使能
        };

        //!@brief ISP信息
        struct Info {
            int chn; //!<通道号
            int iso; //!<感光度，环境越暗值越大
            int redGain; //!<红色增益，环境越暗值越大
            int blueGain; //!<蓝色增益，环境越暗值越大
            int luma; //!<亮度，画面越暗值越小
        };

        /**
         * @brief 设置运行模式
         * @param mode [in] 运行模式
         * @return ErrCodeE
         */
        virtual ErrCodeE SetRunMode(RunModeE mode) = 0;

        /**
         * @brief 获取运行模式
         * @param mode [out] 运行模式
         * @return ErrCodeE
         */
        virtual ErrCodeE GetRunMode(RunModeE &mode) = 0;
    };

}

#endif //EMX_MediaInfIsp_HPP