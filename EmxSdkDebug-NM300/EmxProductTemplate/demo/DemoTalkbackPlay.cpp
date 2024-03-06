//
// Created by xiong on 2023/8/29.
//
#include "EmxMedia.hpp"

using namespace Emx;

// 解码播放实时音频流示例
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage:./TalkbackPlay the/path/to/music.aac\n");
        return 0;
    }
    // 打开aac音频文件，需要保证解码通道配置为aac解码,并且与music文件解码配置一致
    FILE *fp = fopen(argv[1], "rb");
    if (!fp) {
        emxloge("cannot open file %s\n", argv[1]);
        return -1;
    }
    char buffer[2048];
    while (true) {
        // 读取aac音频文件
        auto n = fread(buffer, 1, 7, fp);
        if (n <= 0) {
            fseek(fp, SEEK_SET, 0);
            emxlogd("EOF\n");
            break;
        }
        auto size = ((buffer[3] & 0x03) << 11) | (buffer[4] << 3) | (buffer[5] >> 5);
        n = fread(buffer + 7, 1, size - 7, fp);
        if ((int) n != size - 7) {
            emxloge("Not an ADTS packet\n");
            return -1;
        }

        // 将音频帧发送给MediaServer进行解码播放
        MediaAdec::Frame frame = {};
        frame.data = (uint8_t *) buffer;
        frame.size = (int) size;
        MediaAdecStreamSync::PushFrame(0, frame);
        // 防止读文件的方式发送实时流太快造成缓冲区溢出，这里增加延时
        usleep(20000);
    }
    return 0;
}