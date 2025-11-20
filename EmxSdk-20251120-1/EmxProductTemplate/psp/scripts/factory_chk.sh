#!/bin/sh
FILE="/mnt/sdcard/zn_debug"
MAC_JSON="/root/configs/wifimac.txt"

#mac工厂模式检测
#1，工厂模式；0，正常模式
MacFactoryCheck() {
  if [ -f ${MAC_JSON} ]; then
    size=`ls -l ${MAC_JSON} | awk '{print $5}'`
    echo "mac size:${size}"
    if [ $size -lt 18 ];then
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

