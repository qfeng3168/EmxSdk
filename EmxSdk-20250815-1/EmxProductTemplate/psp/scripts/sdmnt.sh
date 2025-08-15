#!/bin/sh

echo "into sdmnt"

mount /dev/mmcblk0p1 /mnt/sdcard -o errors=continue || mount -o remount,rw /dev/mmcblk0p1

echo "leave sdmnt"
