cmake_minimum_required(VERSION 3.13)  # CMake version check
project(bufferfs
    VERSION 0.1
    LANGUAGES C
)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

add_library(bfs_lib "")

add_subdirectory(include)
add_subdirectory(src)

add_executable(bufferfs bufferfs.cpp)
target_link_libraries(bufferfs PRIVATE bfs_lib)
