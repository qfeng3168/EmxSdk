//
// Created by xiong on 2023/8/29.
//
#include "EmxModules.hpp"

using namespace Emx;

// TimeZone使用示例
int main(int argc,char **argv){
    if(argc != 2){
        printf("Usage: %s <tz>\n", argv[0]);
        return -1;
    }
    int tz = atoi(argv[1]);
    TimeZone time_zone;
    ErrCodeE e = time_zone.Load();
    if (e != ErrCodeE::Success) { 
        emxloge("failed,e=%d\n",e); 
        return -1; 
    }
    char zone[24] = {0};
    if (tz >= 0) {
        snprintf(zone, sizeof(zone) - 1, "GMT-%d", tz);
    } else {
        snprintf(zone, sizeof(zone) - 1, "GMT+%d", abs(tz));
    }

    if(time_zone.Set(zone) != ErrCodeE::Success) { 
        emxloge("failed,e=%d\n",e); 
        return -1; 
    }
    emxlogi("TimeZone set success\n");
    sleep(1);
    emxlogi("TimeZone set success\n");
    sleep(1);
    emxlogi("TimeZone set success\n");
    sleep(1);
    emxlogi("TimeZone set success\n");
    sleep(1);
    emxlogi("TimeZone set success\n");
    sleep(1);
    emxlogi("TimeZone set success\n");
    sleep(1);
    emxlogi("TimeZone set success\n");
    
    return 0;
}