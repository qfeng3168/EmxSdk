#!/bin/sh

WATCHDOG_DEVICE="/dev/watchdog"
# 关闭硬件喂狗和软件喂狗
killall -9 daemon.sh
echo -n V > "$WATCHDOG_DEVICE"
