cmake_minimum_required(VERSION 4.4)
project(app)

function(define_struct size_var align_var)
    set(struct_size 0)
    set(struct_align 1)

    math(EXPR last "${ARGC} - 1")
    foreach(i RANGE 2 ${last} 3)
        math(EXPR i1 "${i} + 1")
        math(EXPR i2 "${i} + 2")
        list(GET ARGV ${i} name)
        list(GET ARGV ${i1} size)
        list(GET ARGV ${i2} align)

        math(EXPR struct_size "(${struct_size} + ${align} - 1) / ${align} * ${align}")
        set(${name} ${struct_size} PARENT_SCOPE)
        math(EXPR struct_size "${struct_size} + ${size}")
        if(${align} GREATER ${struct_align})
            set(struct_align ${align})
        endif()
    endforeach()

    math(EXPR struct_size "(${struct_size} + ${struct_align} - 1) / ${struct_align} * ${struct_align}")
    set(${size_var} ${struct_size} PARENT_SCOPE)
    set(${align_var} ${struct_align} PARENT_SCOPE)
endfunction()

function(add_tac_library lib_name)
    set(objs)
    math(EXPR last "${ARGC} - 1")
    foreach(i RANGE 1 ${last})
        list(GET ARGV ${i} src)
        string(REGEX MATCH "[^/]*$" basename ${src})
        string(REGEX MATCH "[^.]*$" ext ${basename})
        #string(REGEX REPLACE "\\.[^.]*$" ".o" obj ${basename})
        set(obj "${basename}.o")
        if(NOT IS_ABSOLUTE ${src})
            set(src "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
        endif()

        if("${ext}" STREQUAL "s")
            add_custom_command(
                OUTPUT ${obj}
                COMMAND cp ${src} "${basename}.s"
                COMMAND as-- "${basename}.s"
                DEPENDS ${src}
                VERBATIM
            )
        elseif("${ext}" STREQUAL "cmm")
            add_custom_command(
                OUTPUT ${obj}
                COMMAND cp ${src} "${basename}.c"
                COMMAND cc -E -MD -MF "${basename}.d" ${CMCPPFLAGS} "${basename}.c" -o "${basename}.cmm"
                COMMAND cm2e -c ${CMFLAGS} "${basename}.cmm"
                DEPENDS ${src}
                DEPFILE "${basename}.d"
                VERBATIM
            )
        elseif("${ext}" STREQUAL "c")
            add_custom_command(
                OUTPUT ${obj}
                COMMAND cc -E -MD -MF "${basename}.d" -DCMMDEF ${CMCPPFLAGS} ${src} -o "${basename}.cmm"
                COMMAND cm2e -c ${CMFLAGS} "${basename}.cmm"
                DEPENDS ${src}
                DEPFILE "${basename}.d"
                VERBATIM
            )
        endif()
        list(APPEND objs ${obj})
    endforeach()

    add_custom_command(
        OUTPUT "${lib_name}.o"
        COMMAND ld-- "${lib_name}.o" ${objs} > "${lib_name}.sym"
        DEPENDS ${objs}
        VERBATIM
    )
    add_custom_target(${lib_name} ALL DEPENDS "${lib_name}.o")
endfunction()

set(CMCPPFLAGS
    -DEMBEDDED
    -I/usr/local/cmmInclude
    -I/usr/cmmInclude
    -nostdinc
)
set(CMFLAGS
    -K
)
set(CMDIRS
    ../kernel
    ../util
    ../fs
    ../mm
    ../tty
    ../sio
    ../app
    src
    build
)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    list(APPEND CMCPPFLAGS -DDEBUG)
endif()

list(TRANSFORM CMDIRS PREPEND "-I${CMAKE_CURRENT_SOURCE_DIR}/")
list(APPEND CMCPPFLAGS ${CMDIRS})
list(APPEND CMFLAGS ${CMCPPFLAGS})

set(APP_PROCESSES
)
add_executable(generate_process_s)
target_sources(generate_process_s
    PRIVATE
        src/generate_process_s.cpp
)
add_custom_command(
    OUTPUT app_process.s
    COMMAND generate_process_s app_process.s ${APP_PROCESSES}
    VERBATIM
)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    target_compile_definitions(generate_process_s
        PRIVATE
            GENERATE_DEBUG
    )
endif()

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/font.cmm
    COMMAND make --silent -C ${CMAKE_CURRENT_SOURCE_DIR}/Font
    DEPENDS Font/font8x8.c Font/font8x8_basic.h
    VERBATIM
)

define_struct(lcd_size lcd_align
    lcd_buf 1024 2
    lcd_flags 16 2
)

set(KEY_UP 5)
set(KEY_LEFT 4)
set(KEY_RIGHT 3)
set(KEY_BACK 2)
set(KEY_DOWN 1)
set(KEY_ENTER 0)

math(EXPR KEY_UP_MASK "1 << ${KEY_UP}")
math(EXPR KEY_LEFT_MASK "1 << ${KEY_LEFT}")
math(EXPR KEY_RIGHT_MASK "1 << ${KEY_RIGHT}")
math(EXPR KEY_BACK_MASK "1 << ${KEY_BACK}")
math(EXPR KEY_DOWN_MASK "1 << ${KEY_DOWN}")
math(EXPR KEY_ENTER_MASK "1 << ${KEY_ENTER}")
math(EXPR KEY_ALL_MASK "${KEY_UP_MASK} | ${KEY_LEFT_MASK} | ${KEY_RIGHT_MASK} | ${KEY_BACK_MASK} | ${KEY_DOWN_MASK} | ${KEY_ENTER_MASK}")

# configure_file(src/lcd_init.s.in lcd_init.s)
# configure_file(src/lcd_flush.s.in lcd_flush.s)
# configure_file(src/lcd_draw.s.in lcd_draw.s)
configure_file(src/key.h.in key.h)
# configure_file(src/key.s.in key.s)

add_tac_library(mod_0
    spi.cmm
    font.cmm
    lcd.cmm
    mp3Proc.cmm
    shellProc.cmm
    appUtil.cmm
    mp3Files.cmm

    ${CMAKE_CURRENT_BINARY_DIR}/app_process.s
    src/debug.s
    src/debug.c
    src/lcd.c
    src/random.s
    src/key.c
    src/cmmdef.s
    src/algorithm.c
    src/string.c
    src/stacksafe.s
)
