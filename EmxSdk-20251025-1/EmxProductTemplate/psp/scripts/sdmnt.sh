#!/bin/sh

echo "into sdmnt"
TIMEOUT_SEC=3
DISK=/dev/mmcblk0
waited=0

#兼容处理上电设备节点还没有出现的情况
while [ $waited -lt $TIMEOUT_SEC ] && [ ! -b "$DISK" ]; do
    sleep 1
    waited=$((waited + 1))
done

mount -o errors=continue /dev/mmcblk0p1 /mnt/sdcard || mount -o errors=continue /dev/mmcblk0 /mnt/sdcard
if [ $? -eq 0 ];then
    mount -o remount,rw /mnt/sdcard
fi

echo "leave sdmnt"
