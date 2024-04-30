# 将需要编译的源文件添加到ChipSources中并增加头文件目录
include_directories(${CMAKE_SOURCE_DIR}/chip/${ChipManufactory}/${ChipSerial})
# 这里包含插件所需要的算法
set(EmxAiPluginSources ${EmxAiPluginSources}
${CMAKE_SOURCE_DIR}/chip/${ChipManufactory}/${ChipSerial}/MotionDetection.cpp
${CMAKE_SOURCE_DIR}/chip/${ChipManufactory}/${ChipSerial}/FdPdFusion.cpp
${CMAKE_SOURCE_DIR}/chip/${ChipManufactory}/${ChipSerial}/PersonDetection.cpp
)
aux_source_directory(${CMAKE_SOURCE_DIR}/chip/${ChipManufactory}/${ChipSerial}/${AiPluginProfile} EmxAiPluginSources)
include_directories(${CMAKE_SOURCE_DIR}/chip/${ChipManufactory}/${ChipSerial}/${AiPluginProfile})


# 将最终需要链接的库文件添加到ChipLibs中并增加库文件路径
set(EmxAiPluginLibs
#        libz.a
#        libisp.a
#        libae.a
#        libawb.a
#        libaf.a
#        libvenc.a
#        libini.a
#        sys
)
link_directories(${EmxChipPath}/${ChipManufactory}/${ChipSerial}/${ChipSdkVersion}/lib)

# 芯片厂商如果有需要，在这里增加自己的宏定义或编译选项
add_definitions(-DEmxAiPluginDefine)
add_compile_options(-DEmxAiPluginOption)





