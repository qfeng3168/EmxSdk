//
// Created by tangfahong on 2024/3/21.
//

#ifndef EMX_MediaServerDraw_HPP
#define EMX_MediaServerDraw_HPP


#include "MediaServerBase.hpp"
#include "core/euv/EuvTimer.hpp"
#include "media/common/MediaInfDraw.hpp"
#include "media/common/MediaRegion.hpp"
#include <list>

namespace Emx {

    class MediaServerDrawChn : public MediaInfDraw, public MediaServerModuleChnBase {
    public:
        explicit MediaServerDrawChn(int32_t chn) : MediaServerModuleChnBase(MediaModE::Draw, chn) {}

        ~MediaServerDrawChn() override = default;

        ErrCodeE LoadParam() override;

        ErrCodeE SaveParam() override;

        void *GetParam() override { return &m_param; }

        void *GetNewParam() override { return &m_paramNew; }

        int GetParamSize() override { return -1; }

        ErrCodeE GetParamDefaultData(std::string &data) override;

        void NewParamInCome(const char *data, int size) override;

        bool ParamChanged() override;

        void GetParamData(std::string &data) override;

        void UpdateParam() override { m_param = m_paramNew; }

        void ReleaseNewParam() override { m_paramNew.array.clear(); }

        ErrCodeE OnServerCreate() override;

        void OnServerDestroy() override;

        ErrCodeE OnParamChanged() override;
        
    protected:
        /**
         * @brief 获取通道图像大小(叠加的画面大小，例如vpss chn的宽高)
         * @return
         */
        virtual Size GetChnPicSize() = 0;

        struct DrawInfo {
            int id;//!< Draw的唯一标识
            std::shared_ptr<ParamItem> item;//!< param 缓存
        };

        /**
         * @brief 创建Draw画线框
         * @param data [in] 画线框数据
         * return 返回draw id，-1=失败
         */
        virtual int DrawCreateItem(ParamItem& item) {return -1;}
        
         /**
         * @brief 设置Draw画线框
         * @param id [in] 画线框id
         * @param data [in] 画线框数据
         */
        virtual void DrawSetItem(const int id,ParamItem& item) {}
        
        /**
         * @brief 销毁Draw
         * @param data [in] draw id
         */
        virtual void DrawDestroyItem(const int id){}

    private:      

        void CreateAll();

        void DestroyAll();

        void CalcCoordinate(const float &src, float &dst, int obj);

    protected:
        Param *m_pParam;
        Param m_param;
        Param m_paramNew;

    private:
        Size m_chnPicSize;
        std::list<DrawInfo> m_draw;
    };

    class MediaServerDraw : public MediaServerModuleBase {
    public:
        MediaServerDraw() : MediaServerModuleBase(MediaModE::Draw, "draw") {}
    };
}// namespace Emx

#endif//EMX_MediaServerDraw_HPP