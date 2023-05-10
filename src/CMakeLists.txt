add_library(bufferfs_lib
    # library files
    "../include/bufferfs/version.h"
    "../include/bufferfs/vector.h"

    # source files
    "./vector.c"
)

target_include_directories(bufferfs_lib PUBLIC ../include)
