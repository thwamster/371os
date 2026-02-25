cmake_minimum_required(VERSION 4.0)
project(hello_world LANGUAGES C)
file(GLOB sources src/*)
add_executable(hello_world ${sources})