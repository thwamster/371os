cmake_minimum_required(VERSION 4.2.1)
project(371os C)
set(CMAKE_C_CSTANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -lm")

file(GLOB 01 src/01/*.c)

add_executable(01 ${01})
