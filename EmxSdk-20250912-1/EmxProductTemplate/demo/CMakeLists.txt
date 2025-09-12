cmake_minimum_required(VERSION 3.10)

# 将每个源文件编成一个应用程序
aux_source_directory(. DemoSource)
foreach (m ${DemoSource})
    string(REPLACE "/" ";" A ${m})
    list(GET A -1 B)
    string(REPLACE "." ";" C ${B})
    list(GET C 0 N)
    add_executable(${N} ${m})
    target_link_libraries(${N} EmxSdk libjsoncpp.so libuv.so m pthread)
    EmxStrip(${N} ${N})
    install(TARGETS ${N} DESTINATION ${InstallDir}/demo)
endforeach ()
