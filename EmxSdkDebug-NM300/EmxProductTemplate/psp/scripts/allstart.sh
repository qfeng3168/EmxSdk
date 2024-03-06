#!/bin/sh

echo "into allstart"

EmxCoreServer -b
EmxMediaServer -b
SDCardServer -b
UpdateServer -b
SDCardUpdate -b
NetServer -b
NtpClient -b
NightVision -b
EventServer -b
RecordStdServer -b
RtspServer -b
OnvifService -b
#GB28181Service -b
EmxToolsV1 -b
EmxToolsV2 -b
WebServer -b

echo "leave allstart"
