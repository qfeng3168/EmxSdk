//
// Created by tangfahong on 2024/3/22.
//

#ifndef EMX_DefaultDraw_HPP
#define EMX_DefaultDraw_HPP

#include "EmxCore.hpp"
#include "media/server/MediaServerDraw.hpp"
#include <list>

namespace Emx {
    class DefaultOsd;
    class DefaultDraw : public MediaServerDrawChn {
    public:
        /*!
          * @brief 构造什么都不干
          * @param chn
          */
        DefaultDraw(int32_t chn) : MediaServerDrawChn(chn) { m_chn = chn; }

        virtual ~DefaultDraw(){};

    public:
        /**
         * @brief 创建draw总句柄
         * 
         * @return
         */
        ErrCodeE OnCreate() override;
        /**
         * @brief 释放draw总句柄
         */
        void OnDestroy() override;

        /**
         * @brief 获取通道图像大小(叠加的画面大小，例如vpss chn的宽高)
         * @return
         */
        Size GetChnPicSize() override;
        /**
         * @brief 创建Draw画线框
         * @param data [in] 画线框数据
         * return 返回draw id，-1=失败
         */
        virtual int DrawCreateItem(ParamItem &item) override;
        /**
         * @brief 设置Draw画线框
         * @param id [in] 画线框id
         * @param data [in] 画线框数据
         */
        virtual void DrawSetItem(const int id, ParamItem &item) override;
        /**
         * @brief 销毁Draw
         * @param data [in] draw id
         */
        virtual void DrawDestroyItem(const int id) override;

    protected:
        int m_chn = 0;
        int m_vpssGrp;
        int m_vpssChn;
        int m_chnWidth;
        int m_chnHeight;
        int m_lineIdx = 0;//!< draw句柄
    };
}// namespace Emx


#endif//EMX_DefaultDraw_HPP
