//
// Created by xiong on 2023/8/29.
//

#include "EmxMedia.hpp"

#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#ifdef __ARM_NEON
#include <arm_neon.h>
#endif
using namespace Emx;
static int rgbMode = 565;
static int neonEnable = 0;
// 异步实时流获取示例
class VdecDisplay {
public:
    // MediaStreamAsync
    VdecDisplay() {}

    void Create(EuvLoop &loop, char *buffer, int size) {
        // 首先定义通道，并向通道中加入需要监听的媒体帧类型
        MediaStream::Channel channel;
        channel.Add(Emx::MediaFrame::TypeE::Vdec, 0);// 加入通道0
        // 注册当获取到媒体帧时候的回调
        m_stream.Start(loop, buffer, size, channel, std::bind(&VdecDisplay::OnRecvFrame, this, ph_1, ph_2));
        //打开fb0
        struct fb_fix_screeninfo finfo;
        m_fp = open("/dev/fb0", O_RDWR);
        if (m_fp < 0) {
            printf("Error : Can not open framebuffer device\n");
            return;
        }

        if (ioctl(m_fp, FBIOGET_FSCREENINFO, &finfo)) {
            printf("Error reading fixed information\n");
            return;
        }

        if (ioctl(m_fp, FBIOGET_VSCREENINFO, &m_vinfo)) {
            printf("Error reading variable information\n");
            return;
        }
        m_screenBase = (unsigned char *) mmap(0, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, m_fp, 0);
        printf("map len = %d\n", finfo.line_length * m_vinfo.yres);
        m_screenSize = m_vinfo.xres * m_vinfo.yres;
    }

    void Destroy() {
        // 停止媒体帧的监听
        m_stream.Stop();
        // 关闭文件
        munmap(m_screenBase, m_screenSize);
        close(m_fp);
    }

    void Rgb888ToRgb565(unsigned char *rgb888, int rgb888Len) {
        unsigned char *pRgb888 = rgb888;
        unsigned short *pRgb565 = (unsigned short *) m_screenBase;
        uint8_t *pRgb888End = pRgb888 + rgb888Len;
        if (neonEnable) {
            while (pRgb888 < pRgb888End) {
#ifdef __ARM_NEON
                uint8x8x3_t vsrc;
                uint16x8_t vdst;
                vsrc = vld3_u8(pRgb888);
                vdst = vshll_n_u8(vsrc.val[0], 8);
                vdst = vsriq_n_u16(vdst, vshll_n_u8(vsrc.val[1], 8), 5);
                vdst = vsriq_n_u16(vdst, vshll_n_u8(vsrc.val[2], 8), 11);
                vst1q_u16(pRgb565, vdst);
                pRgb565 += 8;
                pRgb888 += 24;
#endif
            }
        } else {
            while (pRgb888 < pRgb888End) {
                const int r = *pRgb888++;
                const int g = *pRgb888++;
                const int b = *pRgb888++;
                *pRgb565++ = (b >> 3) | ((g & 0xFC) << 3) | ((r & 0xF8) << 8);
            }
        }
    }

private:
    // 当收到媒体帧时触发此回调
    void OnRecvFrame(ErrCodeE e, MediaFrame &frame) {
        // 如果获取失败则返回
        if (e != Emx::ErrCodeE::Success) {
            emxlogw("recv stream failed,type=%d,size=%d\n", frame.type, frame.size);
            return;
        }

        //这里只会收到m_stream.Start的时候在channel中Add的帧类型+通道的帧
        if (frame.type == Emx::MediaFrame::TypeE::Vdec) {
            // 收到视频帧

            uint64_t time1 = Time::GetMonoClockMS();
            if (rgbMode == 565) {
                memcpy(m_screenBase, frame.data, frame.size);
            } else {
                Rgb888ToRgb565(frame.data, frame.size);
            }
            emxlogi("rgb888 to rgb656 time:%lld\n", Time::GetMonoClockMS() - time1);
            if (ioctl(m_fp, FBIOPAN_DISPLAY, &m_vinfo) != 0) { printf("FBIOPAN_DISPLAY failed. \n"); }
        }
    }

private:
    MediaStreamAsync m_stream;       // 定义一个异步音视频流的监听句柄
    int m_fp;                        // fb0套接字
    struct fb_var_screeninfo m_vinfo;//fb0 的显示信息
    unsigned char *m_screenBase;
    unsigned int m_screenSize;
};

int main(int argc, char **argv) {
    rgbMode = atoi(argv[1]);
    if (argc > 2) { neonEnable = atoi(argv[2]); }
    EuvLoop loop;    // 定义一个loop用于所有事件的监听
    VdecDisplay demo;// 定义demo对象
    // loop取名为SaveAsyncStream，当loop销毁时执行demo.Destroy()
    loop.Init("VdecDisplay", [&]() { demo.Destroy(); }, Emx::EuvLoop::Dynamic);

    //创建
    char buffer[1024];
    demo.Create(loop, buffer, (int) sizeof(buffer));

    // loop启动
    loop.Start();

    printf("input 'q' to exit\n");
    while ('q' != getchar());

    // 此处会出发loop.Init的时候注册的回调，并执行其中的demo.Destroy()
    loop.StopAndDeInit();

    printf("exit success\n");
    return 0;
}