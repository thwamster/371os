cmake_minimum_required(VERSION 4.0)
project(isis_tests LANGUAGES C ASM)

set(TEST_ELF test_isis.elf)

file(GLOB SOURCES
        *.c
        *.h
        *.S
        ../src/*.c
        ../src/*.h
        ../src/*.S
)

list(FILTER SOURCES EXCLUDE REGEX ".*main\\.c$")
list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.c)

add_executable(${TEST_ELF} ${SOURCES})

target_compile_options(${TEST_ELF} PRIVATE
        -Wall
        -Wextra
        -mcmodel=medany
        -ffreestanding
)

target_link_options(${TEST_ELF} PRIVATE
        -nostdlib
        -T ${CMAKE_CURRENT_SOURCE_DIR}/../src/linker.ld
)

set_target_properties(${TEST_ELF} PROPERTIES
        LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../src/linker.ld
)