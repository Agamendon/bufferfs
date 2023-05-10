cmake_minimum_required(VERSION 3.16)

project(bufferfs
    VERSION 0.1
    LANGUAGES C
)

set(CMAKE_C_STANDARD 17)

add_subdirectory(include)
add_subdirectory(src)

add_executable(bufferfs bufferfs.c)
target_link_libraries(bufferfs PRIVATE bufferfs_lib)