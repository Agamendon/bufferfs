add_library(bufferfs_lib
    # library files
    "../include/vector.h"
    "../include/splay_tree.h"

    # source files
    "./vector.c"
    "./splay_tree.c"
)

target_include_directories(bufferfs_lib PUBLIC ../include)
