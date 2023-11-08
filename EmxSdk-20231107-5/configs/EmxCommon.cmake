# 检查变量是否被定义
macro(EmxCheck name)
    if (NOT ${name})
        message(FATAL_ERROR "${name} not be specified")
    else ()
        message(STATUS "${name}=${${name}}")
    endif ()
endmacro()

macro(EmxSetDefault name def)
    if (NOT ${name})
        set(${name} ${def})
    endif ()
endmacro()

# 构造编译链
EmxSetDefault(Cross x86_64)
if (NOT ${Cross} STREQUAL x86_64)
    if (CrossPath)
        set(CMAKE_C_COMPILER ${CrossPath}/${Cross}-gcc)
        set(CMAKE_CXX_COMPILER ${CrossPath}/${Cross}-g++)
        set(CMAKE_STRIP ${CrossPath}/${Cross}-strip)
    else ()
        set(CMAKE_C_COMPILER ${Cross}-gcc)
        set(CMAKE_CXX_COMPILER ${Cross}-g++)
        set(CMAKE_STRIP ${Cross}-strip)
    endif ()
else ()
    set(CMAKE_STRIP strip)
endif ()

# 用来build之后strip的宏
macro(EmxStrip target filename)
    add_custom_command(
            TARGET
            ${target}
            POST_BUILD
            COMMAND ${CMAKE_STRIP} -s ${filename}
            COMMENT "Strip ${filename} done")
endmacro()

macro(EmxAddDepend FilePath)
    file(READ ${FilePath} json)
    string(JSON depend GET ${json} "depend")
    string(JSON DependLength LENGTH ${depend})
    math(EXPR DependLength "${DependLength} - 1")
    foreach (i RANGE ${DependLength} )
        string(JSON Obj GET ${depend} ${i})
        string(JSON name GET ${Obj} "name")
        string(JSON version GET ${Obj} "version")
        include_directories(${Emx3drPath}/${name}/${version}/${Cross}/include)
        link_directories(${Emx3drPath}/${name}/${version}/${Cross}/lib)
        string(JSON include GET ${Obj} "include")
        string(JSON IncludeLength LENGTH ${include})
        if (IncludeLength)
            math(EXPR IncludeLength "${IncludeLength} - 1")
            foreach (j RANGE ${IncludeLength})
                string(JSON n GET ${include} ${j})
                include_directories(${Emx3drPath}/${name}/${version}/${Cross}/include/${n})
            endforeach ()
        endif ()
    endforeach ()
endmacro()



