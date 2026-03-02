#!/bin/sh
source ${EMX_SCRIPTS_DIR}/function.sh

WATCHDOG_DEVICE="/dev/watchdog"

AddRecoveryLog(){
  logfile="/root/configs/data/daemon.log"
  if [ -f $logfile ];then
    size=$(ls -l $logfile | awk '{print $5}')
    if [ $size -gt 4096 ];then
      rm $logfile
    fi
  fi
  echo "$1[$2]: restart." >> $logfile
  cat /proc/meminfo >> $logfile

  dmesgLogfile="/root/configs/data/dmesg.log"
  if [ -f $dmesgLogfile ];then
    size=$(ls -l $dmesgLogfile | awk '{print $5}')
    if [ $size -gt 4096 ];then
      rm $dmesgLogfile
    fi
  fi
  dmesg >> $dmesgLogfile
  sync
}

AppCheckAndRecovery(){
  App=$(ps | grep $1 | grep -v grep)
  if [[ "${App/$1//}" == "$App" ]];then
    # 写入日志文件
    # AddRecoveryLog "$1" "$2"
    if [[ $1 == "EmxCoreServer" ]];then #|| $1 == "EmxMediaServer"
      # 不可恢复进程，重启系统
      echo "EmxCoreServer restart, reboot"
      reboot -f
    elif [[ $1 == "EmxMediaServer" ]];then
      # 不可恢复进程，重启系统
      if [ -f ${UPDATING_FLAG} ]; then
        #升级时忽略
        echo "updating... ignore EmxMediaServer"
      else
        echo "EmxMediaServer restart, reboot"
        reboot -f
      fi
    elif [[ $1 == "EmxModulesServer" ]];then
      echo "EmxModulesServer restart"
      EmxModulesServer -b
    elif [[ $1 == "DxMain" ]];then
      echo "DxMain restart"
      DxMain -b
    else
      # 服务重新启动
      $1 -b
    fi
  fi
}


# 软件喂狗
SoftwareWatchdog() {
  while true; do
    sleep 10
    currentTime=$(date +"%Y-%m-%d %H:%M:%S")
    AppCheckAndRecovery EmxCoreServer "$currentTime"
    AppCheckAndRecovery EmxMediaServer "$currentTime"
    AppCheckAndRecovery EmxModulesServer "$currentTime"
    if [ -f ${UPDATING_FLAG} ]; then
        #升级时忽略
        echo "updating... ignore DxMain"
    else
        AppCheckAndRecovery DxMain "$currentTime"
    fi
    # AppCheckAndRecovery Launcher "$currentTime"
    # AppCheckAndRecovery RecordServer "$currentTime"
  done
}

SoftwareWatchdog &