cmake_minimum_required(VERSION 3.20)
project(DynamicLibs LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_library(gcd_euclid SHARED src/gcd_euclid.c)
add_library(gcd_naive SHARED src/gcd_naive.c)
add_library(sort_bubble SHARED src/sort_bubble.c)
add_library(sort_quicksort SHARED src/sort_quicksort.c)

target_include_directories(gcd_euclid PUBLIC include)
target_include_directories(gcd_naive PUBLIC include)
target_include_directories(sort_bubble PUBLIC include)
target_include_directories(sort_quicksort PUBLIC include)

add_executable(main_linktime main_linktime.cpp)
target_link_libraries(main_linktime gcd_euclid sort_bubble)

add_executable(main_runtime main_runtime.cpp)
find_package(Threads REQUIRED)
target_link_libraries(main_runtime ${CMAKE_DL_LIBS} Threads::Threads)