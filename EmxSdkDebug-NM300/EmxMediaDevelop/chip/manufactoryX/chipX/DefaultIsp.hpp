//
// Created by xiong on 2023/9/7.
//

#ifndef EMX_DefaultIsp_HPP
#define EMX_DefaultIsp_HPP

#include "media/server/MediaServerIsp.hpp"
#include "EmxCore.hpp"

namespace Emx {

    class DefaultIsp : public MediaServerIspChn {
    public:
        DefaultIsp(int32_t chn) :
                MediaServerIspChn(chn), m_mode(RunModeE::UnDef) {}

        virtual ~DefaultIsp() {};
    public:

        ErrCodeE OnCreate() override;

        void OnDestroy() override;

        ErrCodeE OnParamChanged() override;

        ErrCodeE SetRunMode(RunModeE mode) override;

        ErrCodeE GetRunMode(RunModeE &mode) override {
            mode = m_mode;
            return ErrCodeE::Success;
        }

    protected:
        ErrCodeE Collection(Info &info) override;

    protected:
        int m_pipe;
        RunModeE m_mode;
        Param *m_pParam;
    };

}

#endif //EMX_DefaultIsp_HPP
