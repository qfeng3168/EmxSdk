#!/bin/sh

echo "into appinit"
####################mount cfg####################
ubiattach /dev/ubi_ctrl -m 8
if [ $? != 0  ]; then
  echo 'ubiattach failed, Try ubiformat and ubimkvol'
  ubiformat -y /dev/mtd8
  ubiattach /dev/ubi_ctrl -m 8
  ubimkvol /dev/ubi1 -N CFG -m
fi

if [ ! -c /dev/ubi1_0 ]; then
  mdev -s
fi

mount -t ubifs -o sync /dev/ubi1_0 /root/configs
if [ $? != 0  ]; then
  echo 'Mount /root/configs failed, Try formatting and remounting'
  ubimkvol /dev/ubi1 -N CFG -m
  mount -t ubifs -o sync /dev/ubi1_0  /root/configs
fi

create_dir_if_not_exist() {
    if [ ! -d "$1" ]; then
        mkdir -p "$1"
        echo "Created directory: $1"
    else
        echo "Directory already exists: $1"
    fi
}
create_dir_if_not_exist "/root/configs/AiData"
create_dir_if_not_exist "/root/configs/data"
create_dir_if_not_exist "/root/configs/normal"
create_dir_if_not_exist "/root/configs/backup"
create_dir_if_not_exist "/root/configs/ovd/data"

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
mkdir /tmp/syslog
####################mount sdcard####################
sdmnt.sh
####################network####################
net.sh
create_resolv.sh
####################insert module####################
p=${PWD}
media_insertko.sh
app_insertko.sh
cd /root
####################run deamon####################
cp ${EMX_CONFIGS_DIR}/zoneinfo/GMT-8 /tmp/localtime
init_ai_resources.sh
if [ -f /root/configs/allstart.sh ]; then
  echo "run /root/configs/allstart.sh"
  /root/configs/allstart.sh
else
  echo "run default allstart.sh"
  allstart.sh
fi
#daemon.sh&

echo "leave appinit"
