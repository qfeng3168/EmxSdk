cmake_minimum_required(VERSION 3.19)
#工程名
project(EmxMediaDevelop)

#set(CMAKE_VERBOSE_MAKEFILE ON)

# 构建之前需要指定的变量
if (NOT Profile)
    set(Profile chipXDefault)
endif ()

set(EmxSdkPath ${CMAKE_SOURCE_DIR}/EmxSdk)
set(Emx3drPath ${CMAKE_SOURCE_DIR}/Emx3dr/common)
set(EmxChipPath ${CMAKE_SOURCE_DIR}/Emx3dr/chip)

file(READ profile/${Profile}/configs.json json)
string(JSON ChipManufactory GET ${json} "ChipManufactory")
string(JSON ChipSerial GET ${json} "ChipSerial")
string(JSON ChipSdkVersion GET ${json} "ChipSdkVersion")
string(JSON EmxSdkVersion GET ${json} "EmxSdkVersion")
string(JSON Cross GET ${json} "Cross")
string(JSON CrossPath GET ${json} "CrossPath")

# 下面的变量由芯片厂商在configs.json中填写
set(ChipSources) # 源文件集合
set(ChipLibs) # 依赖的各种库集合
# 加载厂商编写的build.cmake
include(profile/${Profile}/build.cmake)

# 从EmxSdk中加载默认的构造配置
set(EmxSdkVersionPath ${EmxSdkPath}/${EmxSdkVersion})
include(${EmxSdkVersionPath}/configs/EmxCommon.cmake)
EmxAddDepend(${EmxSdkVersionPath}/configs/EmxCoreCfg.json)
EmxAddDepend(${EmxSdkVersionPath}/configs/EmxMediaCfg.json)
include(${EmxSdkVersionPath}/configs/EmxMedia.cmake)
