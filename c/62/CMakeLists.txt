cmake_minimum_required(VERSION 4.0)
project(isis LANGUAGES C ASM)

set(KERNEL_ELF kernel.elf)

file(GLOB SOURCES
        src/*.c
        src/*.h
        src/*.S
)

add_executable(${KERNEL_ELF} ${SOURCES})

target_compile_options(${KERNEL_ELF} PRIVATE
        -Wall
        -Wextra
        -mcmodel=medany
        -ffreestanding
)

target_link_options(${KERNEL_ELF} PRIVATE
        -nostdlib
        -T ${CMAKE_CURRENT_SOURCE_DIR}/src/linker.ld
)

set_target_properties(${KERNEL_ELF} PROPERTIES
        LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/linker.ld
)