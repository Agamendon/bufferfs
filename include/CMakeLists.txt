target_sources(bfs_lib
    PRIVATE
        RTree.hpp
        bfs_buffer.hpp
        bfs_file.hpp
        bfs_functions.hpp
)

target_include_directories(bfs_lib PUBLIC ${CMAKE_CURRECT_LIST_DIR})