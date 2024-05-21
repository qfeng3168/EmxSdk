//
// Created by xiong on 2023/8/24.
//

#ifndef EMX_MediaModulesAll_HPP
#define EMX_MediaModulesAll_HPP

#include "media/common/MediaTypedef.hpp"
#include <map>

namespace Emx {
    class MediaServerModuleBase;

    class MediaModulesAll {
    public:
        static MediaModulesAll *GetInst() {
            static MediaModulesAll m_inst;
            return &m_inst;
        }

        void Create();

        void Destroy();

        bool IsCreated() { return m_created; }

        MediaServerModuleBase *GetModule(MediaModE mod);

    private:
        MediaModulesAll();

    private:
        std::map<MediaModE, MediaServerModuleBase *> m_moduleMap;
        bool m_created;
    };
}// namespace Emx

#endif//EMX_MediaModulesAll_HPP
