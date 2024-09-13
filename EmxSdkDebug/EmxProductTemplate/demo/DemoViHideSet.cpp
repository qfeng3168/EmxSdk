//
// Created by tangfahong on 2024/4/29.
//

#include "EmxMedia.hpp"
#include "media/client/MediaDraw.hpp"

using namespace Emx;
void TestHide2(int show) {

    for (int i = 0; i < 2; i++) {
        MediaInfDraw::Param drawDefPms = {};
        MediaDraw draw(i);
        if (show) {
            {
                std::shared_ptr<MediaInfDraw::ParamItem> item = std::make_shared<MediaInfDraw::ParamItem>();
                item->ena = true;
                item->color = 0x000000ff;
                item->thickness = 2;
                item->type = MediaInfDraw::TypeE::Rect;
                item->rect.fill = 1;
                item->rect.x = 0;
                item->rect.y = 0;
                item->rect.width = 10000;
                item->rect.height = 10000;
                drawDefPms.array.emplace_back(item);
            }
        }

        if (draw.SetTmpParam(drawDefPms) != ErrCodeE::Success) {
            emxloge("set draw info failed!\n");
            return;
        }
        MediaOsd osd(i);
        MediaOsd::Param defOsdPms = {};
        if (osd.GetParam(defOsdPms) != ErrCodeE::Success) { 
            emxloge("get osd info failed!\n"); 
        }
        
        for (auto &item: defOsdPms.array) {
            if (item->type != MediaOsd::TypeE::Time ) { 
                item->ena = false;
            }
            //todo 增加其他text显示“”
        }
        if (osd.SetTmpParam(defOsdPms) != ErrCodeE::Success) { emxloge("set osd info failed!\n"); }
        
        emxlogd("drawDefPms size:%d\n", drawDefPms.array.size());
    }
}

// 视频隐私遮挡配置示例
int main(int argc, char **argv) {
    if (argc < 2) {
        emxloge("Input param failed!eg:%s 1or0 \n",argv[0]);
        return -1;
    }
    // 用于存放参数数据
    MediaVi::Param param = {};
    MediaVi vi(0);
    // 先获取当前的配置
    if (vi.GetParam(param) != ErrCodeE::Success) {
        emxloge("GetParam vi failed\n");
        return -1;
    }
    if(atoi(argv[1])){
        param.hide = true;
    }else{
        param.hide = false;
    }
    if(argc > 2){
        TestHide2(atoi(argv[1]));
        return 0;
    }
    if (vi.SetTmpParam(param) != ErrCodeE::Success) {
        emxloge("SetParam failed\n");
        return -1;
    }
    emxlogi("SetTmpParam vi success\n");
    return 0;
}