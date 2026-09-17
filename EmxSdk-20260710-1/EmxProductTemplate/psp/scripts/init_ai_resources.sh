#!/bin/ash
echo "init_ai_resources"

# 全局变量
g_startTime=$(date +%s)
g_currentDir=$(pwd)
g_sourceFile="${EMX_MEDIA_DIR}/AiPlugin/AiPlugin.json"
g_dataFile="/mnt/data/AiPlugin/AiPlugin.json"
g_destinationFile="${EMX_DATA_DIR}/AiPlugin/AiPlugin.json"

# 计算文件的MD5值
CalculateMd5() {
    local filePath=$1

    if [ -f "$filePath" ]; then
        md5sum "$filePath" | awk '{ print $1 }'
    else
        echo ""
    fi
}

VerifyAllFiles() {
    local jsonFile="${EMX_MEDIA_DIR}/AiPlugin/AiPlugin.json"
    local dataDir="${EMX_DATA_DIR}/AiPlugin"
    local singleItem
    local fileName
    local configMd5
    local actualMd5
    local filePath

    cd "${dataDir}"
    jsonFile="${EMX_MEDIA_DIR}/AiPlugin/AiPlugin.json"
    # 核心步骤：提取files数组并精准拆分为单个文件项
    # 1. 定位files数组区间 2. 过滤所有空白 3. 移除数组首尾标识 4. 按"},{"拆分单条数据
    sed -n '/"files": \[/,/\]/p' "${jsonFile}" | \
        tr -d ' \t\n' | \
        sed -e 's/"files":\[//' -e 's/\]$//' -e 's/},{/\n/g' | \
        while read -r singleItem; do
            # 过滤空行（分割后可能出现，直接跳过）
            [ -z "${singleItem}" ] && continue

            # 提取单个file名称和md5
            fileName=$(echo "${singleItem}" | grep -o '"file":"[^"]*"' | sed 's/"file":"//;s/"$//')
            configMd5=$(echo "${singleItem}" | grep -o '"md5":"[^"]*"' | sed 's/"md5":"//;s/"$//')

            # 过滤解析异常的项（file或md5为空）
            if [ -z "${fileName}" ] || [ -z "${configMd5}" ]; then
                echo "Warning: Invalid file configuration item detected, skipping!"
                continue
            fi

            actualMd5=$(CalculateMd5 "${fileName}")

            # MD5对比，不匹配则输出详细警告
            if [ "${actualMd5}" != "${configMd5}" ]; then
                filePath="${fileName}"

                if [[ "${fileName}" == */* ]]; then
                    if [ ! -d "${dataDir}/${filePath}" ]; then
                        filePath="${fileName%/*}"
                    fi
                fi

                echo "====================================="
                echo "Warning: MD5 check for file ${fileName} does not match!"
                echo "Configured MD5: ${configMd5}"
                echo "Actual MD5: ${actualMd5}"
                echo "Start copy ${filePath}"
                echo "====================================="
                cp -ad "${EMX_MEDIA_DIR}/AiPlugin/${filePath}" "${dataDir}/${filePath}"
            fi
        done

    echo "VerifyAllFiles is done !"
    cd -
}

VerifyConfigsRoFiles() {
    local sourceDir="/mnt/data/AiPlugin/configs_ro"
    local objDir="${EMX_DATA_DIR}/AiPlugin/AiConfigs"
    local sourceFilePath
    local relativePath
    local destinationFilePath
    local sourceMd5
    local destinationMd5
    local destinationDir

    if [ ! -d "${objDir}" ]; then
        mkdir -p "${objDir}"
        echo "${objDir} not exists, create it and copy files from ${sourceDir}"
        cp -ad "${sourceDir}/*" "${objDir}"
        return 0
    fi
    cd "${objDir}"

    # 核心步骤：遍历sourceDir目录下的所有文件，然后和objDir目录下的文件进行对比，需要忽略 config.json param.def param.bak.json 这些文件
    find "${sourceDir}" -type f | while read -r sourceFilePath; do
        relativePath="${sourceFilePath#${sourceDir}/}"

        case "$(basename "${relativePath}")" in
            config.json|param.def|param.bak.json)
                continue
                ;;
        esac

        destinationFilePath="${objDir}/${relativePath}"
        sourceMd5=$(CalculateMd5 "${sourceFilePath}")
        destinationMd5=$(CalculateMd5 "${destinationFilePath}")

        if [ "${sourceMd5}" != "${destinationMd5}" ]; then
            destinationDir="${destinationFilePath%/*}"

            if [ ! -d "${destinationDir}" ]; then
                mkdir -p "${destinationDir}"
            fi

            echo "====================================="
            echo "Warning: configs_ro file ${relativePath} does not match!"
            echo "Source MD5: ${sourceMd5}"
            echo "Destination MD5: ${destinationMd5}"
            echo "Start copy ${relativePath}"
            echo "====================================="
            cp -ad "${sourceFilePath}" "${destinationFilePath}"
        fi
    done

    echo "VerifyConfigsRoFiles is done !"
    cd -
}

if [ -f "${g_dataFile}" ]; then
    echo "${g_dataFile} is exists!"
    VerifyConfigsRoFiles
elif [ -f "${g_sourceFile}" ]; then
    echo "${g_sourceFile} is exists!"
    # 计算源文件和目标文件的MD5值
    sourceMd5=$(CalculateMd5 "${g_sourceFile}")
    destinationMd5=$(CalculateMd5 "${g_destinationFile}")

    # 比较MD5值
    if [ "${sourceMd5}" == "${destinationMd5}" ]; then
        echo "Algorithm resources have not changed"
    else
        echo "AiPlugin.json md5 does not match, re-copy the algorithm resources"
        cd /tmp
        rm -rf "${EMX_DATA_DIR}/AiPlugin"
        
        if [ -f "${g_sourceXzFile}" ]; then
            xz -dkc ${g_sourceXzFile} | tar -xvf - -C ${EMX_DATA_DIR}
        else
        cp -r "${EMX_MEDIA_DIR}/AiPlugin" "${EMX_DATA_DIR}/"
        fi
        sync
        cd "${g_currentDir}"
    fi
    VerifyAllFiles
fi

# 记录结束时间并计算耗时
endTime=$(date +%s)
spentTime=$((endTime - g_startTime))
echo "init_ai_resources spent time: ${spentTime} s"
