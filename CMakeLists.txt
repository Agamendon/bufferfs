cmake_minimum_required(VERSION 3.13)
project(bufferfs
    VERSION 0.1
    LANGUAGES C
) 


set(CMAKE_CXX_STANDARD 11)

add_library(bfs_lib "")
set_target_properties(bfs_lib PROPERTIES LINKER_LANGUAGE C)

add_subdirectory(include)
add_subdirectory(src)

add_executable(bufferfs bufferfs.c)
target_link_libraries(bufferfs PRIVATE bfs_lib)