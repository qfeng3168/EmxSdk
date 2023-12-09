#!/bin/sh

echo "into appinit"

####################pinmux####################
pinmux.sh

####################ulimit####################
ulimit -s unlimited
ulimit -l unlimited
ulimit -p unlimited

####################create dir####################
mkdir -p /var/run/hostapd
mkdir -p /var/lib/misc
touch /var/lib/misc/udhcpd.leases
mkdir /tmp/net
mkdir /tmp/webserver

####################mount sdcard####################
sdmnt.sh
####################network####################
net.sh
telnetd
####################insert module####################
p=${PWD}
cd ${EMX_MEDIA_DIR}/drivers
loadko
cd /root
####################run deamon####################
cp ${EMX_CONFIGS_DIR}/zoneinfo/GMT-8 /tmp/localtime
#allstart.sh
#daemon.sh&

echo "leave appinit"
