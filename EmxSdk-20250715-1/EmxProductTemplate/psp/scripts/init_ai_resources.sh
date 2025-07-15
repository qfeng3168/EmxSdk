#!/bin/ash
echo "init_ai_resources"

# 计算文件的MD5值
calculate_md5() {
    local file_path=$1
    if [ -f "$file_path" ]; then
        md5sum "$file_path" | awk '{ print $1 }'
    else
        echo ""
    fi
}

# 记录开始时间
start_time=$(date +%s)

# 保存当前工作目录
current_dir=$(pwd)

# 文件路径
source_file="/root/firmware/media/AiPlugin/AiPlugin.json"
destination_file="/root/configs/AiData/AiPlugin/AiPlugin.json"

# 计算源文件和目标文件的MD5值
source_md5=$(calculate_md5 "$source_file")
destination_md5=$(calculate_md5 "$destination_file")

# 比较MD5值
if [ "$source_md5" == "$destination_md5" ]; then
    echo "Algorithm resources have not changed"
else
    echo "AiPlugin.json md5 does not match, re-copy the algorithm resources"
    cd /tmp
    rm -rf /root/configs/AiData/*
    cp -r /root/firmware/media/AiPlugin /root/configs/AiData/
    sync
    cd "$current_dir"
fi

# 记录结束时间
end_time=$(date +%s)

# 计算并输出耗时
spent_time=$((end_time - start_time))
echo "init_ai_resources spent time: $spent_time s"