include(ExternalProject)

function(add_target type)
    cmake_parse_arguments(TYPE "TEST" "" "" ${ARGN})

    if (${type} MATCHES "host")
        file(GLOB sources src/*)
        add_executable(${PROJECT_NAME} ${sources})
        target_link_libraries(${PROJECT_NAME} m)

    elseif (${type} MATCHES "riscv64-toolchain")
        set(executable_name ${PROJECT_NAME}_kernel.elf)
        set(base_executable_name ${PROJECT_NAME}_base_kernel.elf)
        set(directory_src ${CMAKE_CURRENT_SOURCE_DIR}/src)
        set(directory_scripts ${CMAKE_SOURCE_DIR}/../../py)

        if (TYPE_TEST)
            set(executable_name ${PROJECT_NAME}_test_kernel.elf)
            set(base_executable_name ${PROJECT_NAME}_base_test_kernel.elf)
            set(directory_src ${CMAKE_CURRENT_SOURCE_DIR}/../src)
            set(directory_scripts ${CMAKE_SOURCE_DIR}/../../../py)
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

        add_executable(${base_executable_name} ${SOURCES})

        target_compile_options(${base_executable_name} PRIVATE
                -g -Wall -Wextra -mcmodel=medany -ffreestanding -fno-omit-frame-pointer
        )
        target_link_options(${base_executable_name} PRIVATE
                -nostdlib -T ${directory_src}/linker.ld
        )
        set_target_properties(${base_executable_name} PROPERTIES
                LINK_DEPENDS ${directory_src}/linker.ld
        )

        set(symbol_c ${CMAKE_CURRENT_BINARY_DIR}/symbol_table.c)
        add_custom_command(
                OUTPUT ${symbol_c}
                COMMAND riscv64-unknown-elf-nm -n ${CMAKE_CURRENT_BINARY_DIR}/${base_executable_name} > ${CMAKE_CURRENT_BINARY_DIR}/symbols.txt
                COMMAND python3 ${directory_scripts}/get_symbols.py ${CMAKE_CURRENT_BINARY_DIR}/${base_executable_name} ${CMAKE_CURRENT_BINARY_DIR}/symbols.txt ${symbol_c}
                DEPENDS ${base_executable_name}
        )

        add_executable(${executable_name} ${SOURCES} ${symbol_c})

        target_compile_options(${executable_name} PRIVATE
                -g -Wall -Wextra -mcmodel=medany -ffreestanding -fno-omit-frame-pointer
        )
        target_link_options(${executable_name} PRIVATE
                -nostdlib -T ${directory_src}/linker.ld
        )
        set_target_properties(${executable_name} PROPERTIES
                LINK_DEPENDS ${directory_src}/linker.ld
        )
    endif ()
endfunction()