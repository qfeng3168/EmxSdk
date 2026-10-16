#!/bin/sh
#通用函数接口定义 其他文件如需引用 添加：source ${EMX_SCRIPTS_DIR}/function.sh  即可
UPDATING_FLAG="/tmp/updating"
UPDATE_FILE="/mnt/sdcard/rom.bin"
MAC_JSON="/root/configs/normal/mac.json"
SDCARD_DEBUG_FILE_FLAG="/mnt/sdcard/zn_debug"
SDCARD_AIDEBUG_FILE_FLAG="/mnt/sdcard/ai_debug"

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
    sleep 0.5
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

# 通用配置检查函数
# 参数 $1: 要查询的字段名 (例如: coredump_mode)
# 返回值: 0 (True/存在且为true), 1 (False/不存在或不为true)
IsConfigTrue() {
    local target_key="$1"
    local config_file="$SDCARD_DEBUG_FILE_FLAG"

    # 1. 检查参数是否为空
    if [ -z "$target_key" ]; then
        echo "Error: No config key provided."
        return 1 # False
    fi

    # 2. 检查配置文件是否存在
    if [ ! -f "$config_file" ]; then
        # 调试打印，实际使用可视情况注释掉
        # echo "Debug file not found: $config_file"
        return 1 # False
    fi

    # 3. 使用扩展正则 grep -E 进行精确匹配
    # ^[[:space:]]*      : 匹配行首，允许有空格
    # ${target_key}      : 传入的字段名
    # [[:space:]]*=[[:space:]]* : 匹配等号，允许等号两边有空格
    # true               : 匹配值 true
    # \b                 : 单词边界，防止匹配到 true_value 这种情况
    # -i                 : 忽略大小写 (可选，如果 value 必须是小写 true 则去掉 -i)
    if grep -qE "^[[:space:]]*${target_key}[[:space:]]*=[[:space:]]*true\b" "$config_file"; then
        return 0 # Shell标准：0 代表成功 (True)
    else
        return 1 # Shell标准：非0 代表失败 (False)
    fi
}

#zn_debug工厂模式检测
#1，工厂模式；0，正常模式
ZndebugFactoryCheck() {
    if IsConfigTrue "factory_mode"; then
        return 1
    else
        return 0
    fi
}

#mac工厂模式检测
#1，工厂模式；0，正常模式
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
IsUpdating() {
    if [ -f ${UPDATING_FLAG} ]; then
        return 1
    else
        return 0
    fi
    return 0
}

#是否调试模式 0,非调试模式;1,调试模式
IsDebugMode() {
    if [ -f ${SDCARD_DEBUG_FILE_FLAG} ]; then
        return 1
    else
        return 0
    fi
    return 0
}

#是否Ai调试模式 0,非调试模式;1,调试模式
IsAiDebugMode() {
    if [ -f ${SDCARD_AIDEBUG_FILE_FLAG} ]; then
        return 1
    else
        return 0
    fi
    return 0
}

#是否coredump数据模式 0,非抓取模式;1,抓取模式
IsCoreDumpMode() {
    if IsConfigTrue "coredump_mode"; then
        return 1
    else
        return 0
    fi
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

#nand检测并挂载到configs分区 参数说明：$1=分区号 $2=配置初始化标志
#eg:
#	NandConfigCheckMount 7 >> /tmp/NandConfigCheckMount.log
NandConfigCheckMount(){
    local mtdblk=/dev/mtd$1
    local ubiblk=/dev/ubi$1
    local mtdblkEnv='mtd'$1'FsckFailNum'
    local mountPath="/root/configs"   
    local configInit=$(BootEnvTool get configInit)
    local ubiblk_0=$ubiblk'_0'
	local result=0

    # 检查设备是否存在
    if [ ! -c "$mtdblk" ]; then
        echo "Error: Device $mtdblk does not exist first, try!!!"
        mdev -s
        sync
        sleep 1
    fi

    if [ ! -c "$mtdblk" ]; then
        echo "Error: Device $mtdblk does not exist, return!!!"
        return -1
    fi
    
    echo 'Check '$mtdblk' configInitFlag: '$configInit
    # 挂载ubi设备
    ubiattach /dev/ubi_ctrl -m $1 -d $1
    if [ $? != 0  ]; then
        if [ "$configInit" == "1" ]; then
            echo 'ubiattach failed, Try ubiattach $1'
            mdev -s
            sync
            sleep 1
            ubiattach /dev/ubi_ctrl -m $1 -d $1
        else
            echo 'ubiattach failed, Try ubiformat and ubimkvol'
            # 格式化ubi设备,这里可能会出现卡死的情况，需要使用timeout做超时处理
            # 如果30秒都没有处理完，那么就需要特殊处理了
            timeout -s 9 30 ubiformat -y $mtdblk
            if [ $? != 0  ]; then
                echo 'ubiformat failed, Please fill in the data and try again.'
                ep_mtd_ctl erase $mtdblk 0 0
                timeout -s 9 30 ubiformat -y $mtdblk
            fi
            ubiattach /dev/ubi_ctrl -m $1 -d $1
            ubimkvol $ubiblk -N CFG -m
            mdev -s
        fi
    else
        # 如果configInit不是1，就检查是否存在configs分区
        if [ "$configInit" != "1" ]; then
            # 如果configs分区不存在，就创建
            echo 'configs partition is not existing, try create it'
            if [ ! -c "$ubiblk_0" ]; then
                ubimkvol $ubiblk -N CFG -m
                mdev -s
            fi
        fi
    fi
    # UBI设备节点创建可能滞后，挂载前统一扫描并等待，避免正常重启时误判节点不存在。
    mdev -s
    sync
    sleep 1
    if [ ! -c "$ubiblk_0" ]; then
        echo "Error: Device $ubiblk_0 does not exist"
        cur=$(BootEnvTool get $mtdblkEnv | awk '{print $1}')
        cur=$((cur + 1))
        echo $mtdblkEnv':'$cur
        BootEnvTool set $mtdblkEnv $cur
        result=1
    else
        echo 'Check successfully' $ubiblk_0
        mount -t ubifs -o sync $ubiblk_0  $mountPath
        result=$?
        if [ $result == 0 ]; then
            echo 'Mount successfully' $ubiblk_0
            if [ "$configInit" != "1" ]; then
                BootEnvTool set configInit 1
            fi
        else
            echo 'Mount failed' $ubiblk_0
        fi
    fi
    return $result
}
