//
// Created by xiong on 2023/8/29.
//
#include "EmxMedia.hpp"
#include <thread>

using namespace Emx;

int stream2645Parse(void *pData, int *ps32ReadLen) {
    int s32Ret = 0;
    uint8_t *pu8Buf = (uint8_t *)pData;
    int i = 0;
    bool bFindStart = false;
    bool bFindEnd = false;

    for (i = 0; i < *ps32ReadLen - 8; i++) {
        if (pu8Buf[i] == 0 && pu8Buf[i + 1] == 0 && pu8Buf[i + 2] == 0 && pu8Buf[i + 3] == 1) {
            bFindStart = true;
            i += 128;
            break;
        }
    }

    for (; i < *ps32ReadLen - 8; i++) {
        if (pu8Buf[i] == 0 && pu8Buf[i + 1] == 0 && pu8Buf[i + 2] == 0 && pu8Buf[i + 3] == 1) {
            bFindEnd = true;
            break;
        }
    }

    if ((bFindStart == false) || (bFindEnd == false)) {
        return -1;
    }

    if (i > 0) {
        *ps32ReadLen = i;
    }

    return s32Ret;
}


int main(int argc,char **argv) {
    MediaVdecStream * vdecStream = new MediaVdecStream(0);
    MediaVdec::Param param = {};
    MediaVdec vdec(0);
    if(argc < 2){
        emxloge("argc num err!eg:./DemoVdecStream 1.h265(or 1.h264) fps(test fps)\n");
        return -1;
    }
    // 先获取当前的配置
    if (vdec.GetParam(param) != ErrCodeE::Success) {
        emxloge("GetParam failed\n");
        return -1;
    }
    int loopNum = 0;
    if(strstr(argv[1],"h265")){
        param.codec = VideoCodecE::H265;
    }else{
        param.codec = VideoCodecE::H264;
    }
    if(argc > 2){
        if(strstr(argv[2],"fps")){
            loopNum = 1;
        }
    }
    if (vdec.SetParam(param) != ErrCodeE::Success) {
        emxloge("SetParam failed\n");
        return -1;
    }
   
    vdecStream->Destroy();
    vdecStream->Create();
    int s32Ret = 0; 
    int bufSize = 4147200;
    int s32UsedBytes = 0;
    char * pu8Buf = (char *)malloc(bufSize);
    if (pu8Buf == NULL) {
		printf("%d in send stream thread!\n", bufSize);
		return -1;
	}
    FILE *fpStrm = fopen(argv[1],"rb");
    if(!fpStrm){
        printf("%s open failed!\n", argv[1]);
		return -1;
    }
    struct timeval tv;
    unsigned long long ddd;
    gettimeofday(&tv, (struct timezone *) 0);
    ddd = (unsigned long long) tv.tv_sec * 1000 + (unsigned long long) tv.tv_usec / 1000;
    int realFps = 0;
    uint64_t pts = 1;
    printf("input 'q' to exit\n");
    while (loopNum ||('q' != getchar())){
        fseek(fpStrm, s32UsedBytes, SEEK_SET);
        int s32ReadLen = fread(pu8Buf, 1, bufSize, fpStrm);
        if (s32ReadLen == 0) {
            s32UsedBytes = 0;
            fseek(fpStrm, 0, SEEK_SET);
            s32ReadLen = fread(pu8Buf, 1, bufSize, fpStrm);
        }

        s32Ret = stream2645Parse(pu8Buf, &s32ReadLen);
        if (s32Ret != 0) {
            if (s32ReadLen >= bufSize) {
                printf("can not find H264 start code! "
                    "s32ReadLen %d, s32UsedBytes %d.!\n",
                    s32ReadLen, s32UsedBytes);
            } else {
                 printf("No a complete framee! ");
            }
        }

        MediaInfVdec::FrameData frameOut = {};
        frameOut.tsInUs = pts++;
        frameOut.data.data = pu8Buf ;
        frameOut.data.size = s32ReadLen;
        vdecStream->PushFrame(frameOut,1000);
        realFps++;
        s32UsedBytes = s32UsedBytes + s32ReadLen;
        gettimeofday(&tv, (struct timezone *) 0);
        unsigned long long fff = (unsigned long long) tv.tv_sec * 1000 + (unsigned long long) tv.tv_usec / 1000;
        if(fff - ddd >= 1000){
            ddd = fff;
            printf("Real-time frame rate=%d\n",realFps);
            realFps = 0;
        }
    }

    printf("exit success\n");
    return 0;
}
