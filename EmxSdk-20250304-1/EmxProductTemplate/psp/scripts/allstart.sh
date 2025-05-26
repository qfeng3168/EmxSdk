#!/bin/sh

echo "into allstart"

runExe() {
    if [ $# -lt 1 ]; then
        echo -e "\033[1;31mERR: require at least input EXE name!!\033[0m"
        return
    fi

    printf "\033[1;32m"
    printf "#################################\n"
    printf "#   START %-21s #\n" $1
    printf "#################################"
    printf "\033[0m\n"

    # Run EXE bin here
    $@
    sleep 1
}

#################### WatchDog ####################
runExe WatchDog -b
#################### core dump ####################
ulimit -c unlimited;
echo "/mnt/sdcard/core-%e-%p-%t" > /proc/sys/kernel/core_pattern;
echo 1 > /proc/sys/kernel/core_uses_pid
cat /proc/sys/kernel/core_pattern;
#core run
runExe EmxCoreServer -b
runExe NetServer -b
runExe EmxMediaServer -b
runExe NightVision -b
runExe SDCardServer -b
runExe UpdateServer -b
runExe SDCardUpdate -b
runExe EmxToolsV2 -b
runExe RtspServer -b

echo "leave allstart"
