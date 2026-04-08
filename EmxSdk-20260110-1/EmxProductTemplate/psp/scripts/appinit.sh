#!/bin/sh
echo "into appinit"
source ${EMX_SCRIPTS_DIR}/function.sh
####################nand flash mount cfg####################
NandConfigCheckMount 8
if [ $? != 0  ]; then
  echo '\n'
  echo '**********************configs mount failed!!!****************************'
  echo '\n'
fi

#################### emmc mount config####################
# configInitFlag=$(BootEnvTool get configInit)
# echo "mount /root/configs start"
# EmmcExt4CheckMount 7 /root/configs rw 0 $configInitFlag >> /tmp/EmmcExt4CheckMount.log
# mount1=$?
# #如果只读备份roconfigs能挂载，但是configs能挂载，就需要将configs格式化，然后配置恢复
# if [ $mount1 -ne 0 ]; then
#     echo "Configs damage, start formatting, and then restore data." >> /tmp/EmmcExt4CheckMount.log
# 	mkfs.ext4 -F /dev/mmcblk0p7 
# 	mount -o rw -t ext4 /dev/mmcblk0p7 /root/configs
#     sync
# fi
# echo "mount /root/configs down"

# ####################configs备份####################
# MacFactoryCheck
# isMacFactoryMode=$?
# echo "mount /root/roconfigs start"
# if [ $isMacFactoryMode -eq 0 ]; then
#     EmmcExt4CheckMount 8 /root/roconfigs rw 0 $configInitFlag >> /tmp/EmmcExt4CheckMount.log
#     echo "normal mode"
#     #如果/root/roconfigs/normal不存在,则将/root/configs 中的文件复制到/root/roconfigs备份
#     if [ ! -d "/root/roconfigs/normal" ]; then
#         cp /root/configs/* /root/roconfigs/ -ad
#         echo "copy /root/configs to /root/roconfigs done" >> /tmp/EmmcExt4CheckMount.log
#         sync
#     fi
# else
#     #没有烧号工厂模式如果/mnt/emmc存在roconfigs,则将/mnt/emmc/roconfigs 中的文件复制到/root/configs 还原
#     #因为有可能/rootfs/configs  出现异常但是被修复了，所以需要重置一下
#     EmmcExt4CheckMount 8 /root/roconfigs rw 0 $configInitFlag >> /tmp/EmmcExt4CheckMount.log
#     if [ -d "/root/roconfigs/normal" ]; then
#         echo "configs file system has been damaged and needs to be reset." >> /tmp/EmmcExt4CheckMount.log
# 		cp /root/roconfigs/* /root/configs/ -ad
#         echo "copy /root/roconfigs to /root/configs done" >> /tmp/EmmcExt4CheckMount.log
#         sync
#     fi
# fi
# # umount /root/roconfigs
# # echo "umount /root/roconfigs done" >> /tmp/EmmcExt4CheckMount.log

# echo "mount /mnt/emmc start"
# EmmcExt4CheckMount 9 /mnt/emmc rw 1 $configInitFlag >> /tmp/EmmcExt4CheckMount.log
# mount2=$?
# echo "mount /mnt/emmc done"

# if [ $mount1 -ne 0 ] || [ $mount2 -ne 0 ]; then
#     echo 'Mount  disk  failed, please check the disk.'
# else
#     echo "Mount disk success"
#     if [ "$configInitFlag" != "1" ]; then
#         echo "Write config init flag"
#         BootEnvTool set configInit 1
#     fi
# fi

####################create dir####################
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
create_dir_if_not_exist "/var/run/hostapd"
create_dir_if_not_exist "/var/lib/misc"
create_dir_if_not_exist "/tmp/net"
create_dir_if_not_exist "/tmp/webserver"
create_dir_if_not_exist "/tmp/syslog"
create_dir_if_not_exist "/root/configs/utmp/root"

touch /var/lib/misc/udhcpd.leases
####################pinmux####################
pinmux.sh

####################ulimit####################
#ulimit -s unlimited
ulimit -l unlimited
####################mount sdcard####################
sdmnt.sh
####################network####################
net.sh
create_resolv.sh
####################insert module####################
media_insertko.sh
app_insertko.sh
cd /root
####################run deamon####################
cp ${EMX_CONFIGS_DIR}/zoneinfo/GMT-8 /tmp/localtime
init_ai_resources.sh

#allstart
if [ -f /root/configs/allstart.sh ]; then
  echo "run /root/configs/allstart.sh"
  /root/configs/allstart.sh &
elif [ -f /mnt/sdcard/allstart.sh ]; then
  echo "run /mnt/sdcard/allstart.sh"
  /mnt/sdcard/allstart.sh &
else
  echo "run default allstart.sh"
  allstart.sh &
fi

echo "leave appinit"
