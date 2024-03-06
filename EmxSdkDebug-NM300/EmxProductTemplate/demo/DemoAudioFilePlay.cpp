//
// Created by xiong on 2023/8/29.
//

#include "EmxMedia.hpp"

using namespace Emx;

// 播放音频文件示例
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage:./DemoAudioFilePlay the/path/to/music.aac\n");
        printf("support format .aac/.wav(codec=alaw/ulaw/pcm)/.pcm\n");
        return 0;
    }
    // 播放指定目录下的音频文件,可以多次重复调用，MediaServer会按照先后顺序组织播放队列进行播放
    MediaAdecStreamSync::PlayFile(0, argv[1]);
    return 0;
}