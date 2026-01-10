#!/bin/sh

#开机启动时，往/etc/resolv.conf写入默认DNS解析服务器地址
#解决单栈IPV6获取到地址后，没有域名解析服务
#udhcpc程序只在IPV4情况下生效

RESOLV_FILE="/tmp/resolv.conf"
echo "write default dns addr:${RESOLV_FILE}"
echo "nameserver 114.114.114.114" >> ${RESOLV_FILE}
echo "write default dns addr done"
