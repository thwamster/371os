cmake_minimum_required(VERSION 4.0)
project(malloc LANGUAGES C)
file(GLOB sources src/*)
add_executable(my_malloc ${sources})