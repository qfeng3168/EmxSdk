//
// Created by xiong on 2023/8/29.
//

#include "EmxMedia.hpp"

using namespace Emx;

// 视频编码配置示例，音频/ISP等使用SetParam/GetParam接口进行参数配置的类都可参考此示例
int main(int argc, char **argv) {
    if (argc < 2) {
        emxloge("Usage: %s modeName <json_file>\n", argv[0]);
        emxloge("modeName: 模式名称,eg:aenc adec vi isp venc vdec vo ai\n");
        emxloge("json_file: json文件路径\n");
        return -1;
    }
    int chn = 0;
    Json::Value data = {};
    if(argc > 2) {
        EasyJson::Load(argv[2], data);
    }else{
        data["cmd"] = "test";
        data["param"] = 999;
        data["codec"] = 365;
    }
    if(argc > 3) {
        chn = atoi(argv[3]);
    }
    std::string modeName = argv[1];
    Json::Value out;
    ErrCodeE result = ErrCodeE::Failure;
    if(modeName == "aenc"){
        MediaAenc mode(chn);
        result = mode.DirectControl(data, out);
    }else if(modeName == "adec"){
        MediaAdec mode(chn);
        result = mode.DirectControl(data, out);
    }else if(modeName == "vi"){
        MediaVi mode(chn);
        result = mode.DirectControl(data, out);
    }else if(modeName == "isp"){
        MediaIsp mode(chn);
        result = mode.DirectControl(data, out);
    }else if(modeName == "venc"){
        MediaVenc mode(chn);
        result = mode.DirectControl(data, out);
    }else if(modeName == "vdec"){
        MediaVdec mode(chn);
        result = mode.DirectControl(data, out);
    }else if(modeName == "vo"){
        MediaVo mode(chn);
        result = mode.DirectControl(data, out);
    }else if(modeName == "ai"){
        MediaAi mode(chn);
        result = mode.DirectControl(data, out);
    }else{
        emxloge("Unknown mode %s\n", modeName.c_str());
        return -1;
    }

    if (result != ErrCodeE::Success) {
        emxloge("DirectControl failed\n");
        return -1;
    }
    emxlogi("DirectControl cmd:%s,chn:%d success\n", modeName.c_str(), chn);
    emxlogi("DirectControl out:%s\n", out.toStyledString().c_str());
    return 0;
}
