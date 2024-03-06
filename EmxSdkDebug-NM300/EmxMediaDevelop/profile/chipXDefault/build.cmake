# 将需要编译的源文件添加到ChipSources中并增加头文件目录
aux_source_directory(${CMAKE_SOURCE_DIR}/chip/${ChipManufactory}/${ChipSerial} ChipSources)
include_directories(${CMAKE_SOURCE_DIR}/chip/${ChipManufactory}/${ChipSerial})
aux_source_directory(${CMAKE_SOURCE_DIR}/chip/${ChipManufactory}/${ChipSerial}/${Profile} ChipSources)
include_directories(${CMAKE_SOURCE_DIR}/chip/${ChipManufactory}/${ChipSerial}/${Profile})
aux_source_directory(${EmxChipPath}/${ChipManufactory}/${ChipSerial}/${ChipSdkVersion}/sample ChipSources)
include_directories(${EmxChipPath}/${ChipManufactory}/${ChipSerial}/${ChipSdkVersion}/sample)
include_directories(${EmxChipPath}/${ChipManufactory}/${ChipSerial}/${ChipSdkVersion}/include)

# 将最终需要链接的库文件添加到ChipLibs中并增加库文件路径
set(ChipLibs
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
add_definitions(-DEmxMediaDefine)
add_compile_options(-DEmxMediaOption)





