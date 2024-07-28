#!/bin/sh

echo "into daemon"

AddRecoveryLog(){
  logfile="/tmp/daemon.log"
  if [ -f $logfile ];then
    size=$(ls -l $logfile | awk '{print $5}')
    if [ $size -gt 4096 ];then
      rm $logfile
    fi
  fi
  echo "restart $1 on `date`" >> $logfile
}

AppCheckAndRecovery(){
  App=$(ps | grep $1 | grep -v grep)
  #application disappeared
  if [[ "${App/$1//}" == "$App" ]];then
    $1 $2
    AddRecoveryLog $1
  fi
}

while true
do
  # if the core service EmxCoreServer is not running, restart all
  App=$(ps | grep EmxCoreServer | grep -v grep)
  if [[ "${App/EmxCoreServer//}" == "$App" ]];then
    allstop.sh
    allstart.sh
    AddRecoveryLog "all"
  else
    AppCheckAndRecovery EmxCoreServer
#    AppCheckAndRecovery EmxMediaServer
#    AppCheckAndRecovery SDCardServer
#    AppCheckAndRecovery UpdateServer
#    AppCheckAndRecovery NetServer
#    AppCheckAndRecovery NtpClient
#    AppCheckAndRecovery NightVision
#    AppCheckAndRecovery EventServer
#    AppCheckAndRecovery RecordStdServer
#    AppCheckAndRecovery RtspServer
#    AppCheckAndRecovery OnvifService
#    AppCheckAndRecovery GB28181Service
#    AppCheckAndRecovery EmxToolsV2
#    AppCheckAndRecovery WebServer
  fi
    sleep 5
done

echo "leave daemon"
