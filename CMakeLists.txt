cmake_minimum_required(VERSION 4.0)
project(371os C)
set(CMAKE_C_CSTANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

add_executable(my_wc src/wc.c) 
