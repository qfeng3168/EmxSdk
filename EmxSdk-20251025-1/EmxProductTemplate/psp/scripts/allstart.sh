#!/bin/sh

echo "into allstart"
source ${EMX_SCRIPTS_DIR}/function.sh

#工厂模式检测
AppFactoryCheck
isFactoryMode=$?
echo "isFactoryMode:$isFactoryMode"

debug_mode=0
CrtChk 
if [ $? -eq 0 ]; then
  debug_mode=1
  serial_switch.sh 1
  echo "Program 'CrtChk' executed successfully."
else
  if [ $isFactoryMode -eq 1 ];then
    echo "factory mode result"
    debug_mode=1
    #serial_switch.sh 1
  else
    echo "close serial port"
    serial_switch.sh 0
  fi
fi

echo "debug_mode:${debug_mode}"
if [ $debug_mode -eq 1 ];then
    telnetd &
    echo "debug mode!!!"
    umount /mnt/sdcard;
    sleep 0.5
    umask 077;
    ulimit -c unlimited;
    echo "/mnt/sdcard/core-%e-%p-%t" > /proc/sys/kernel/core_pattern;
    echo 1 > /proc/sys/kernel/core_uses_pid
    cat /proc/sys/kernel/core_pattern;
    sdmnt.sh
    sleep 0.5
fi

#app run
runExe EmxCoreServer -b
runExe EmxMediaServer -b
runExe EmxModulesServer -b

if [ -f /mnt/sdcard/zn_debug_v2 ];then
    runExe EmxToolsV2 -b
fi

if [ $debug_mode -eq 1 ];then
    run EmxFctToolsV1 -b
    run RtspServer -b
    telnetd &
    aging.sh &
fi

if [ $isFactoryMode -eq 0 ];then
    runExe DxMain -b
    runExe OtaServer -b
    daemon.sh &
fi

echo "leave allstart"