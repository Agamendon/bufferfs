target_sources(bfs_lib
    PRIVATE
        bfs_buffer.cpp
        bfs_file.cpp
)

target_include_directories(bfs_lib PUBLIC ${CMAKE_CURRECT_LIST_DIR})