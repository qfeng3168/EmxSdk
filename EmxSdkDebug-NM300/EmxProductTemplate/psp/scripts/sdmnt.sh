#!/bin/sh

echo "into sdmnt"

mount -t vfat /dev/mmcblk0p1 /mnt/sdcard || mount -t vfat /dev/mmcblk0 /mnt/sdcard
if [ $? -eq 0 ];then
    mount -o remount,rw /mnt/sdcard
fi

echo "leave sdmnt"
