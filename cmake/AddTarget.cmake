include(ExternalProject)

function(add_target type)
    cmake_parse_arguments(TYPE "TEST" "" "" ${ARGN})

    if (${type} MATCHES "host")
        file(GLOB sources src/*)
        add_executable(${PROJECT_NAME} ${sources})
        target_link_libraries(${PROJECT_NAME} m)

    elseif (${type} MATCHES "riscv64-toolchain")
        set(executable_name ${PROJECT_NAME}_kernel.elf)
        set(directory_src ${CMAKE_CURRENT_SOURCE_DIR}/src)

        if (TYPE_TEST)
            set(executable_name ${PROJECT_NAME}_test_kernel.elf)
            set(directory_src ${CMAKE_CURRENT_SOURCE_DIR}/../src)
        endif ()

        file(GLOB SOURCES
                *.c
                *.h
                *.S
                ${directory_src}/*.c
                ${directory_src}/*.h
                ${directory_src}/*.S
        )

        if (TYPE_TEST)
            list(FILTER SOURCES EXCLUDE REGEX ".*main\\.c$")
            list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.c)
        endif ()

        add_executable(${executable_name} ${SOURCES})

        target_compile_options(${executable_name} PRIVATE
                -Wall
                -Wextra
                -mcmodel=medany
                -ffreestanding
        )

        target_link_options(${executable_name} PRIVATE
                -nostdlib
                -T ${directory_src}/linker.ld
        )

        set_target_properties(${executable_name} PROPERTIES
                LINK_DEPENDS ${directory_src}/linker.ld
        )
    endif ()
endfunction()