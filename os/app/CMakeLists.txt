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

        if(${ext} STREQUAL "s")
            add_custom_command(
                OUTPUT ${obj}
                COMMAND cp ${src} .tmp.s
                COMMAND as-- .tmp.s
                COMMAND mv .tmp.o ${obj}
                DEPENDS ${src}
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
