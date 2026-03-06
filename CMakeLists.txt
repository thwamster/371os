cmake_minimum_required(VERSION 4.0)
set(CMAKE_C_CSTANDARD 23)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(371os LANGUAGES C ASM)

enable_testing()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(AddComponent)

add_subdirectory(c/01)
add_subdirectory(c/21)
add_subdirectory(c/22)
add_subdirectory(c/62)
add_subdirectory(c/62/tests)

add_component(01 my_wc host Ninja)
add_component(21 my_malloc host Ninja)
add_component(22 hello_world host Ninja)
add_component(62 isis riscv64-toolchain Ninja)
