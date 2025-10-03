cmake_minimum_required(VERSION 3.10)
project(FirstOSLAb)

set(CMAKE_C_STANDARD 99)

add_executable(parent src/parent.c)
add_executable(child src/child.c)

target_compile_options(parent PRIVATE -Wall -Wextra)
target_compile_options(child PRIVATE -Wall -Wextra)