cmake_minimum_required(VERSION 3.10)
project(FirstOSLAb)

set(CMAKE_C_STANDARD 99)

add_executable(parent parent.c)
add_executable(child child.c)

target_compile_options(parent PRIVATE -Wall -Wextra)
target_compile_options(child PRIVATE -Wall -Wextra)