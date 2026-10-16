#!/bin/sh

echo "into mkfs"

/sbin/mdev -s
mkexfatfs $1

echo "leave mkfs"
