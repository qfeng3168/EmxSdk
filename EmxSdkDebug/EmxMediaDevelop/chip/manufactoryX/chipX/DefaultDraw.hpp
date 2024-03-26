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
        DefaultDraw(int32_t chn) : MediaServerDrawChn(chn) {m_chn = chn;}

        virtual ~DefaultDraw(){};

    public:
        /**
         * @brief 创建draw总句柄
         * 这里是初始化一个最大的画布，但是依赖osd，要注意两者的使用
         * @return
         */
        ErrCodeE OnCreate() override;
        /**
         * @brief 释放draw总句柄
         */
        void OnDestroy() override;

        Size GetChnPicSize() override;

        virtual int DrawCreateItem(ParamItem& item) override;
        
        virtual void DrawSetItem(const int id,ParamItem& item) override;
        
        virtual void DrawDestroyItem(const int id) override;

    protected:
        int m_chn = 0;
        int m_vpssGrp;
        int m_vpssChn;
        int m_chnWidth;
        int m_chnHeight;
        int m_lineIdx =0 ; //!< draw句柄
    };
}// namespace Emx


#endif//EMX_DefaultDraw_HPP
