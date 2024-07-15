#!/bin/sh

echo "into allstop"

killall -q WebServer -b
killall -q EmxToolsV2 -b
#killall -q GB28181Service -b
killall -q OnvifService -b
killall -q RtspServer -b
killall -q RecordStdServer -b
killall -q EventServer -b
killall -q NightVision -b
killall -q NtpClient -b
killall -q NetServer -b
killall -q SDCardUpdate -b
killall -q UpdateServer -b
killall -q SDCardServer -b
killall -q EmxMediaServer -b
killall -q EmxCoreServer -b

echo "leave allstop"
