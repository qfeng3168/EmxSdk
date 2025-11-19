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
