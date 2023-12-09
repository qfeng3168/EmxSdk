//
// Created by xiong on 2023/8/29.
//

#include "EmxModules.hpp"

using namespace Emx;
// 固件升级示例
int main() {
    // 打开固件文件
    FILE *fp = fopen("rom.bin", "rb");
    if (!fp) {
        printf("open failed\n");
        return -1;
    }
    UpdateClient update; // 定义升级对象
    update.Create(4534324); // 创建升级任务，自己指定一个session，用于区分升级操作的来源
    uint8_t buffer[4096];
    int n;
    auto e = ErrCodeE::Failure;
    while ((n = (int) fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        e = update.Push(buffer, n); // 向UpdateServer发送固件
        if (e != Emx::ErrCodeE::Success && e != Emx::ErrCodeE::ReachTheEof) {
            printf("update failed\n");
            break;
        }
    }
    update.Destroy();
    fclose(fp);
    if (n < 0 || (e != Emx::ErrCodeE::Success && e != Emx::ErrCodeE::ReachTheEof)) {
        printf("rom send failed\n");
    } else {
        printf("rom send done\n");
    }
    return 0;
}