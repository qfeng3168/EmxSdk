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

        
        void CreateSingle(ParamItem &param);

        void DestroySingle(int id);
    protected:
        /**
         * @brief 获取通道图像大小(叠加的画面大小，例如vpss chn的宽高)
         * @return
         */
        virtual Size GetChnPicSize() = 0;

        struct DrawInfo {
            bool show; //!<是否显示
            int layer;//!<显示层级
            std::vector<MediaInfDraw::Line> line;
            int id;//!< Draw的唯一标识
        };

        /**
         * @brief 创建Draw
         * @param data [in] 画线框数据
         */
        virtual void DrawCreateAll(std::list<DrawInfo> &data) {}
        /**
         * @brief 销毁Draw
         */
        virtual void DrawDestroyAll(const std::list<DrawInfo> &data){};

        /**
         * @brief 暂停Draw通道，用于编码通道切换分辨率时停止，可视情况选择是否实现
         */
        virtual void Pause() {}

        /**
         * @brief 恢复Draw通道，用于编码通道切换分辨率后恢复，可视情况选择是否实现
         */
        virtual void Resume() {}

    private:      

        void CreateAll();

        void DestroyAll();

        void CalcCoordinate(std::vector<MediaInfDraw::Line> &src, std::vector<MediaInfDraw::Line> &dst);

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