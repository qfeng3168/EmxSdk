//
// Created by xiong on 2023/8/29.
//
#include "EmxMedia.hpp"
#include "EmxModules.hpp"

using namespace Emx;

// 用于监听大部分事件
class ListenEvent {
public:
    void Create() {
        m_loop.Init("ListenEvent", [this]() { OnQuit(); });
        // 注册升级监听事件
        m_updateEvent.Create(&m_loop, m_buffer, sizeof(m_buffer), std::bind(&ListenEvent::OnUpdateEvent, this, ph_1));
        // 注册SDCard监听事件
        m_sdcardEvent = new SDCardClientEvent(m_loop, m_buffer, sizeof(m_buffer),
                                              std::bind(&ListenEvent::OnSDCardEvent, this, ph_1, ph_2));
        m_sdcardEvent->Create();

        // 定义一个key的gpio其中的num是sys/class/gpio/中的gpio号
        // m_keyGpio = new Gpio(408, Emx::Gpio::DirE::In, Emx::Gpio::ActiveE::Low, Gpio::ValueE::Disable);
        // 创建key，可以给这个key命名一下，传入绑定的loop，传入需要监听的key，传入长按的判断依据时长，按下超过3秒触发长按
        m_key = new Key("DemoKeyTest", m_loop, m_bsp.keyReset.get(), 3000);
        // 注册key的监听回调，当按键被短按或者长按的时候触发此回调
        m_key->Create(std::bind(&ListenEvent::OnKeyPressed, this, ph_1, ph_2));

        // 创建一个QR扫描时间句柄，监听通道sensor0的扫码
        m_qr = new MediaQrScan(0);
        // 注册一个回调函数，当扫描到有效的二维码后触发
        m_qr->StartQR(m_loop, m_buffer, sizeof(m_buffer),
                      std::bind(&ListenEvent::OnQrScanningResult, this, ph_1, ph_2));

        // 注册一个回调函数，当接收到Isp状态信息后触发
        m_isp.Start(m_loop, m_buffer, sizeof(m_buffer),
                    std::bind(&ListenEvent::OnIspInfo, this, ph_1));
        m_loop.Start();
    }

    void Destroy() {
        m_loop.StopAndDeInit();
    }

private:
    void OnQuit() {
        // 销毁各种监听
        m_isp.Stop();
        m_updateEvent.Destroy();
        m_sdcardEvent->Destroy();
        delete m_sdcardEvent;
        m_key->Destroy();
        delete m_key;
        m_qr->StopQR(); //停止扫码
        delete m_qr;
        delete m_keyGpio;
    }

private:
    // 当SD卡状态发生变化的时候触发此回调
    void OnUpdateEvent(UpdateEvent::Event &e) {
        emxlogi("stat=%d,session=%d,download[%d/%d],burn=[%d/%d]\n",
                (int) e.stat, e.session, e.download.current, e.download.total, e.burn.current, e.burn.total);
    }

    // 当SD卡状态发生变化的时候触发此回调
    void OnSDCardEvent(int chn, SDCard::Info &info) {
        emxlogi("chn[%d] dev[%s] stat[%d] fileSystem[%d] total[%u] free[%u] used[%u] mountPoint[%s]\n",
                chn, info.dev, (int) info.stat, (int) info.fileSystem, info.total, info.free, info.used,
                info.mountPoint);
    }

    // 当按键被短按或者长按的时候触发此回调
    void OnKeyPressed(const char *name, Key::EventE e) {
        emxlogi("Key[%s] pressed\n", name);
        if (e == Key::EventE::Click) // 短按触发
            emxlogi("key Click triggered\n");
        else if (e == Key::EventE::LongPress) // 长按触发
            emxlogi("key LongPress triggered\n");
    }

    // 当扫描到有效的二维码后触发
    void OnQrScanningResult(int chn, const char *result) {
        emxlogi("chn %d get QR content#%s#\n", chn, result);
    }

    // 当扫描到有效的二维码后触发
    void OnIspInfo(MediaIsp::Info &info) {
        emxlogd("chn=%d, luma=%d, iso=%d, redGain=%d, blueGain=%d\n",
                info.chn, info.luma, info.iso, info.redGain, info.blueGain);
    }

private:
    EuvLoop m_loop;
    char m_buffer[4096];
    Gpio *m_keyGpio;
    Key *m_key; // 定义一个key的监听句柄
    UpdateEvent m_updateEvent; // 用于注册监听升级事件
    SDCardClientEvent *m_sdcardEvent; // 用于注册监听SDCard事件
    MediaQrScan *m_qr; // 定义一个二维码扫描对象用来执行扫码
    MediaIspInfo m_isp; // 定义一个isp对象用来接收ISP消息
    Bsp m_bsp;
};

int main() {
    ListenEvent demo; // 定义demo对象
    //创建
    demo.Create();
    printf("input 'q' to exit\n");
    while ('q' != getchar());
    demo.Destroy();
    printf("exit success\n");
    return 0;
}