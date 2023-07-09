add_library(bufferfs_lib
    # library files
    "../include/vector.h"
    "../include/splay_tree.h"
    # "../include/file_buffer.h"

    # source files
    "./vector.c"
    "./splay_tree.c"
    # "./file_buffer.c"
)

target_include_directories(bufferfs_lib PUBLIC ../include)
