cmake_minimum_required(VERSION 3.19)

# 自动使用当前目录名作为工程名
string(REPLACE "/" ";" PathList ${CMAKE_SOURCE_DIR})
list(GET PathList -1 TopDirName)
project(${TopDirName})

# 固定配置
set(EmxSdkPath ${CMAKE_SOURCE_DIR}/Public/EmxSdk)
set(Emx3drPath ${CMAKE_SOURCE_DIR}/Public/Emx3dr/common)
if (NOT InstallDir)
    set(InstallDir ${CMAKE_SOURCE_DIR}/output/debug)
endif ()
set(InstallBin ${InstallDir}/bin)
set(InstallLib ${InstallDir}/lib)

# 加载产品配置
file(READ product.json json)
string(JSON EmxSdkVersion GET ${json} "EmxSdkVersion")
file(READ psp/psp.json json)
string(JSON Cross GET ${json} "Cross")
string(JSON CrossPath GET ${json} "CrossPath")

set(EmxSdkVersionPath ${EmxSdkPath}/${EmxSdkVersion})
include(${EmxSdkVersionPath}/configs/EmxCommon.cmake)
EmxAddDepend(${EmxSdkVersionPath}/configs/EmxCoreCfg.json)

# 添加sdk头文件和库文件
include_directories(${EmxSdkVersionPath}/include)
link_directories(${EmxSdkVersionPath}/cross/${Cross}/lib)

# 加载编译配置
include(build.cmake)
