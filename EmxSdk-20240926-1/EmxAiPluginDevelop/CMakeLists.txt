cmake_minimum_required(VERSION 3.19)
#工程名
project(EmxAiPluginDevelop)

#set(CMAKE_VERBOSE_MAKEFILE ON)

# 构建之前需要指定的变量
if (NOT AiPluginProfile)
    set(AiPluginProfile chipXDefault)
endif ()

set(EmxSdkPath ${CMAKE_SOURCE_DIR}/Public/EmxSdk)
set(Emx3drPath ${CMAKE_SOURCE_DIR}/Public/Emx3dr/common)
set(EmxChipPath ${CMAKE_SOURCE_DIR}/Public/Emx3dr/chip)

file(READ profile/${AiPluginProfile}/profile.json json)
string(JSON ChipManufactory GET ${json} "ChipManufactory")
string(JSON ChipSerial GET ${json} "ChipSerial")
string(JSON ChipAiVersion GET ${json} "ChipAiVersion")
string(JSON ChipSdkVersion GET ${json} "ChipSdkVersion")
string(JSON EmxSdkVersion GET ${json} "EmxSdkVersion")
string(JSON Cross GET ${json} "Cross")
string(JSON CrossPath GET ${json} "CrossPath")

# 下面的变量由芯片厂商在configs.json中填写
set(EmxAiPluginSources) # 源文件集合
set(EmxAiPluginLibs) # 依赖的各种库集合
# 加载厂商编写的build.cmake
include(profile/${AiPluginProfile}/build.cmake)

# 从EmxSdk中加载默认的构造配置
set(EmxSdkVersionPath ${EmxSdkPath}/${EmxSdkVersion})
include(${EmxSdkVersionPath}/configs/EmxCommon.cmake)
EmxAddDepend(${EmxSdkVersionPath}/configs/EmxCoreCfg.json)

####################################################################
# 定义EmxMediaServer依赖信息
add_definitions(-DChipManufactory=\"${ChipManufactory}\")
add_definitions(-DChipSerial=\"${ChipSerial}\")
add_definitions(-DAiPluginProfile=\"${AiPluginProfile}\")
add_definitions(-DChipAiVersion=\"${ChipAiVersion}\")
add_definitions(-DChipSdkVersion=\"${ChipSdkVersion}\")
add_definitions(-DEmxMediaVersion=\"${EmxMediaVersion}\")
add_definitions(-DEmxAiPluginVersion=\"${EmxAiPluginVersion}\")
add_definitions(-DEmxSdkVersion=\"${EmxSdkVersion}\")

# 通用的编译选项
add_compile_options(
        -g
        -Wall
        -O2
        -fPIC
        -Werror
        -Wno-unused-result
        -Wno-format-truncation
        -Wno-stringop-truncation
        -Wno-format-overflow)

# 加载sdk依赖
include_directories(${EmxSdkVersionPath}/include)
link_directories(${EmxSdkVersionPath}/cross/${Cross}/lib)

# EmxAddDepend(${EmxSdkVersionPath}/configs/EmxCoreCfg.json)

# 构建libEmxAiPlugin.so动态库
add_library(EmxAiPlugin SHARED ${EmxAiPluginSources})
EmxStrip(EmxAiPlugin libEmxAiPlugin.so)

# EmxMediaServer依赖的库文件
target_link_libraries(EmxAiPlugin EmxSdk
        ${EmxAiPluginLibs}
        libjsoncpp.so libuv.so
        m pthread)

# 安装动态库和程序编译使用的profile配置文件
if (NOT InstallDir)
    set(InstallDir ${CMAKE_SOURCE_DIR}/output)
endif ()
install(DIRECTORY profile/${AiPluginProfile} DESTINATION ${InstallDir})
install(TARGETS EmxAiPlugin DESTINATION ${InstallDir}/${AiPluginProfile})

