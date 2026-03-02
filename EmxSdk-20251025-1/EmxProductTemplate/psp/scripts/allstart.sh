#!/bin/sh

echo "into allstart"
source ${EMX_SCRIPTS_DIR}/function.sh
debug_mode=0
run WatchDog -b

# support ipV6 dns parse
create_resolv.sh

# ---------- 工厂模式检测 ----------
AppFactoryCheck
isFactoryMode=$?
echo "isFactoryMode:$isFactoryMode"

# ---------- 证书校验与串口控制 ----------
CrtChk
if [ $? -eq 0 ]; then
  debug_mode=1
  serial_switch.sh 1
  echo "Program 'CrtChk' executed successfully."
else
  if [ $isFactoryMode -eq 1 ];then
    #工厂模式默认打开串口
    echo "factory mode result"
    debug_mode=1
    serial_switch.sh 1
  else
    echo "close serial port"
    serial_switch.sh 0
  fi
fi

# ---------- CoreDump 配置 ----------
IsCoreDumpMode
isCoreDumpMode=$?
echo "isCoreDumpMode:${isCoreDumpMode}"
if [ $isCoreDumpMode -eq 1 ];then
    telnetd &
    echo "debug mode!!!"
    umount /mnt/sdcard;
    umask 077;
    ulimit -c unlimited;
    echo "/mnt/sdcard/core-%e-%p-%t" > /proc/sys/kernel/core_pattern;
    echo 1 > /proc/sys/kernel/core_uses_pid
    cat /proc/sys/kernel/core_pattern;
    sdmnt.sh
fi

# ---------- Debug 模式检测 ----------
IsDebugMode
isDebugMode=$?
echo "isDebugMode:${isDebugMode}"
if [ $isDebugMode -eq 1 ];then
    debug_mode=1
fi

# ---------- 启动核心服务 ----------
runExe EmxCoreServer -b
runExe EmxMediaServer -b
runExe EmxModulesServer -b

# ---------- 启动调试/产测协议 ----------
start_time=$(date +%s)
echo "debug_mode:$debug_mode"
if [ $debug_mode -eq 1 ];then
    run RtspServer -b
    run EmxFctToolsV1 -b
    run EmxToolsV2 -b
    # run EmxToolsV1 -b
    telnetd &
    if [ $isFactoryMode -eq 1 ];then
        aging.sh & #工厂模式启动老化进程
    fi
fi

# ---------- 卡刷升级检测预留时间 ----------
end_time=$(date +%s)
duration=$((end_time - start_time))
echo "duration:$duration"
#sd卡存在rom.bin文件
if [ -f $UPDATE_FILE ]; then
    interval=5
    #预留时间给到卡刷升级检测
    if [ "$duration" -lt $interval ]; then
        sleepTime=$(($interval - $duration))
        if [ $sleepTime -lt 1 ];then
            sleepTime=1
        fi
        echo "sleepTime:$sleepTime"
        sleep $sleepTime
    fi
fi

# ---------- 启动主业务 ----------
IsUpdating
isUpdate=$? 
echo "IsUpdating:$isUpdate"
if [ $isUpdate -eq 1 ];then
    #已经在升级时不启动主进程，加快升级速度
    echo "is updating, do nothing!!!!!"
else
    if [ $isFactoryMode -eq 0 ];then
        # runExe RecordServer -b
        # runExe Launcher -b
        if [ -f /mnt/sdcard/DxMain ]; then
            run /mnt/sdcard/DxMain -b
            echo "run custom DxMain"
        else #正常运行程序
            runExe DxMain -b
            daemon.sh &
            echo "run default DxMain"
        fi
        # runExe OtaServer -b
    fi
fi

echo "leave allstart"
