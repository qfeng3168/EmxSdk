//
// Created by xiong on 2023/8/29.
//

#include "EmxMedia.hpp"
#include "media/client/MediaDraw.hpp"
using namespace Emx;

static void TestDraw(int show) {

    for (int i = 0; i < 2; i++) {
        MediaInfDraw::Param drawDefPms = {};
        MediaDraw draw(i);
        if (show) {
            {
                std::shared_ptr<MediaInfDraw::ParamItem> item = std::make_shared<MediaInfDraw::ParamItem>();
                item->ena = true;
                item->color = 0xff0000ff;
                item->thickness = 2;
                item->type = MediaInfDraw::TypeE::Rect;
                item->rect.fill = 0;
                item->rect.x = 0;
                item->rect.y = 0;
                item->rect.width = 4000;
                item->rect.height = 4000;
                drawDefPms.array.emplace_back(item);
            }
            {
                std::shared_ptr<MediaInfDraw::ParamItem> item = std::make_shared<MediaInfDraw::ParamItem>();
                item->ena = true;
                item->color = 0xff0000ff;
                item->thickness = 2;
                item->type = MediaInfDraw::TypeE::Line;
                item->line.sp.x = 0;
                item->line.sp.y = 0;
                item->line.ep.x = 10000;
                item->line.ep.y = 10000;
                drawDefPms.array.emplace_back(item);
            }
        }

        if (draw.SetTmpParam(drawDefPms) != ErrCodeE::Success) {
            emxloge("set draw info failed!\n");
            return;
        }
        emxlogd("drawDefPms size:%d\n", drawDefPms.array.size());
    }
}

// OSD配置示例
int main(int argc ,char **argv) {
    TestDraw(atoi(argv[1]));
    return 0;
}