# 定义EmxMediaServer依赖信息
add_definitions(-DChipManufactory=\"${ChipManufactory}\")
add_definitions(-DChipSerial=\"${ChipSerial}\")
add_definitions(-DProfile=\"${Profile}\")
add_definitions(-DChipSdkVersion=\"${ChipSdkVersion}\")
add_definitions(-DEmxMediaVersion=\"${EmxMediaVersion}\")

# 定义EmxMediaServer使用的profile相关的宏定义
add_definitions(-DMEDIA_PROFILE_FACTORY_CLASS=${Profile}Factory)
add_definitions(-DMEDIA_PROFILE_FACTORY_HEADER=\"${Profile}Factory.hpp\")
message(MEDIA_PROFILE=${Profile})
message(MEDIA_PROFILE_FACTORY_CLASS=${Profile}Factory)
message(MEDIA_PROFILE_FACTORY_HEADER=\"${Profile}Factory.hpp\")

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

EmxAddDepend(${EmxSdkVersionPath}/configs/EmxCoreCfg.json)
EmxAddDepend(${EmxSdkVersionPath}/configs/EmxMediaCfg.json)

# 构建EmxMediaServer应用程序
add_executable(EmxMediaServer EmxMedia.cpp ${ChipSources})
EmxStrip(EmxMediaServer EmxMediaServer)

# EmxMediaServer依赖的库文件
target_link_libraries(EmxMediaServer     
        -Wl,--start-group
        EmxMediaServerLib EmxSdk
        ${ChipLibs}
        libfdk-aac.a
        libzbar.a
        libfreetype.a
        libz.a
        -Wl,--end-group
        libjsoncpp.so libuv.so
        m pthread)

# 安装应用程序和程序编译使用的profile配置文件
if (NOT InstallDir)
    set(InstallDir ${CMAKE_SOURCE_DIR}/output)
endif ()
install(DIRECTORY profile/${Profile} DESTINATION ${InstallDir})
install(TARGETS EmxMediaServer DESTINATION ${InstallDir}/${Profile})
