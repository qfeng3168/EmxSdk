#!/bin/sh
#通用函数接口定义 其他文件如需引用 添加：source ${EMX_SCRIPTS_DIR}/function.sh  即可

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

#没有延迟函数
run() {
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
    # sleep 0.2
}

#zn_debug工厂模式检测
#1，工厂模式；0，正常模式
ZndebugFactoryCheck() {
    # 检查文件是否存在
    if [ ! -f "$FILE" ]; then
        echo "Error: File '$FILE' not found."
        return 0
    fi

    # 1. 使用 grep 查找包含 "factory_mode" 的行
    # 2. 使用 sed 移除所有空格，并移除可能存在的分号
    # 3. 使用 grep -q 精确匹配 "factory_mode=true"
    #    -q 选项表示安静模式，只通过退出码表示是否找到
    if grep "factory_mode" "$FILE" | sed 's/[[:space:]]//g' | sed 's/;//' | grep -q "factory_mode=true"; then
        echo "Found 'factory_mode'"
        return 1
    else
        echo "Did not find 'factory_mode'"
        return 0
    fi
}

#mac工厂模式检测
#1，工厂模式；0，正常模式
MAC_JSON="/root/configs/normal/mac.json"
MacFactoryCheck() {
  if [ -f ${MAC_JSON} ]; then
    size=`ls -l ${MAC_JSON} | awk '{print $5}'`
    echo "mac.json size:${size}"
    if [ $size -lt 30 ];then
      echo "factory mode!!!"
      return 1
    else
      echo "normal mode!!!"
      return 0
    fi
  else
    echo "no mac file, factory mode!!!"
    return 1
  fi
}

AppFactoryCheck() {
    MacFactoryCheck
    macFactoryCheck=$?
    echo "macFactoryCheck:$macFactoryCheck"
    if [ $macFactoryCheck -eq 1 ];then
        echo "factory mode macFactoryCheck"
        return 1
    else
        ZndebugFactoryCheck
        zndebugFactoryCheck=$?
        echo "zndebugFactoryCheck:$zndebugFactoryCheck"
        if [ $zndebugFactoryCheck -eq 1 ];then
            echo "factory mode zndebugFactoryCheck"
            return 1
        else 
            echo "no factory mode"
            return 0
        fi 
    fi
}

#是否正在升级中 0,未升级;1,正在升级
UPDATING_FLAG="/tmp/updating"
IsUpdating() {
    if [ -f ${UPDATING_FLAG} ]; then
        return 1
    else
        return 0
    fi
    return 0
}

#是否调试模式 0,非调试模式;1,调试模式
DEBUG_FILE_FLAG="/mnt/sdcard/zn_debug"
IsDebugMode() {
    if [ -f ${DEBUG_FILE_FLAG} ]; then
        return 1
    else
        return 0
    fi
    return 0
}
#emmc检测并挂载 参数说明：$1=分区号 $2=挂载路径 $3=挂载模式(ro/rw) $4=是否快速格式化(0/1) $5=配置初始化标志
#eg:
#	configInitFlag=$(BootEnvTool get configInit)
#	EmmcExt4CheckMount 7 /root/configs rw 0 $configInitFlag >> /tmp/EmmcExt4CheckMount.log
EmmcExt4CheckMount(){
    local mmcblk=/dev/mmcblk0p$1
    local mmcblkEnv='mmc'$1'FsckFailNum'
    local mountPath=$2
    local mountMode=$3
    local quickformat=$4
    local configInit=$5
    local result=0
	local checkFlag=0

    # 检查设备是否存在
    if [ ! -b "$mmcblk" ]; then
        echo "Error: Device $mmcblk does not exist"
        return 0
    fi
    
    echo 'Check '$mmcblk' configInitFlag: '$configInit 'quickformat: '$quickformat
    #如果configInit是1，就检查文件系统
    if [ "$configInit" == "1" ]; then
        fsck.ext4 -n $mmcblk
		checkFlag=$?
    else
        #如果不是就查看是否存在文件系统,存在返回0
        blkid $mmcblk | grep $mmcblk
		checkFlag=$?
        #如果存在文件系统，就检查文件系统是否正常
        if [ $checkFlag -eq 0 ]; then
            echo 'File system is existing:' $mmcblk 
            fsck.ext4 -n $mmcblk
		    checkFlag=$?
        fi
    fi
    if [ $checkFlag -ne 0  ]; then
        echo 'Check '$mmcblk' failed!'
        #检测到文件系统异常，并且configInit不是1，就格式化
        if [ "$configInit" != "1" ]; then
            echo 'Try formatting' $mmcblk
            if [ "$quickformat" = "1" ]; then
                echo 'Quick format' $mmcblk
                mkfs.ext4 -F -O sparse_super,large_file -b 4096 -g 32768 -E nodiscard -J size=128 $mmcblk
            else
                mkfs.ext4 -F $mmcblk
            fi
            if [ $? -eq 0 ]; then
                echo 'Format successfully' $mmcblk 
            else    
                echo 'Format failed' $mmcblk
            fi
            # 验证修复是否成功
            fsck.ext4 -n $mmcblk
            if [ $? -eq 0 ]; then
                echo 'Format Fix successfully' $mmcblk
            else    
                echo 'Format Fix failed' $mmcblk
                result=1
            fi
        else
            #尝试修复
            fsck.ext4 -yf $mmcblk
            # 验证修复是否成功
            fsck.ext4 -n $mmcblk
            if [ $? -eq 0 ]; then
                echo 'Fix successfully' $mmcblk
            else    
                echo 'Fix failed' $mmcblk
                result=1
            fi
      fi
    else
        echo 'Check successfully' $mmcblk
    fi
    #如果修复失败，需要写入环境变量失败次数
    if [ "$result" -ne 0 ]; then
        echo 'Fix failed, try fix and mount'
        #尝试再次修复
        fsck.ext4 -yf $mmcblk
    fi
    # 尝试挂载文件系统
    mount -o $mountMode -t ext4 $mmcblk $mountPath
    if [ $? != 0  ]; then
        # 挂载失败，且没法修复
        echo 'Mount '$mountPath' failed, The partition cannot be repaired.'
        cur=$(BootEnvTool get $mmcblkEnv | awk '{print $1}')
        cur=$((cur + 1))
        echo $mmcblkEnv':'$cur
        BootEnvTool set $mmcblkEnv $cur
        result=1
    else
        echo 'Mount '$mountPath' successfully'
        result=0
    fi
    return $result
}