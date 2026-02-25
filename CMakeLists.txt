cmake_minimum_required(VERSION 4.0)
set(CMAKE_C_CSTANDARD 23)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(371os LANGUAGES C ASM)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(AddMultiTargetComponent)

add_multi_target_component(c 01 host Ninja my_wc)
add_multi_target_component(c 21 host Ninja my_malloc)
add_multi_target_component(c 22 host Ninja hello_world)
add_multi_target_component(c 52 riscv64-toolchain Ninja isis)