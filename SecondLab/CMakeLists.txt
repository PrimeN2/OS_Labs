include_directories(include)

find_package(Threads REQUIRED)

add_executable(second_lab_demo main.c src/timer.c src/parallel_merge_sort.c)

target_link_libraries(second_lab_demo PRIVATE Threads::Threads)
