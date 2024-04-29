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
    // 先获取当前的配置
    if (vdec.GetParam(param) != ErrCodeE::Success) {
        emxloge("GetParam failed\n");
        return -1;
    }
    if(strstr(argv[1],"h265")){
        param.codec = VideoCodecE::H265;
    }else{
        param.codec = VideoCodecE::H264;
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
    uint64_t pts = 1;
    printf("input 'q' to exit\n");
    while ('q' != getchar()){
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
        s32UsedBytes = s32UsedBytes + s32ReadLen;
        printf("send frame size=%d,pts=%lld \n",s32ReadLen,frameOut.tsInUs);
    }

    printf("exit success\n");
    return 0;
}
