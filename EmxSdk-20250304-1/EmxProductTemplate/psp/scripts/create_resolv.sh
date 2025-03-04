#!/bin/sh

RESOLV_FILE="/tmp/resolv.conf"
echo "write default dns addr:${RESOLV_FILE}"
echo "nameserver 114.114.114.114" >> ${RESOLV_FILE}
echo "write default dns addr done"
