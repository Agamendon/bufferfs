# Here some of the dependencies are downloaded

include(FetchContent)


#
# xxhash
#
set(XXHASH_GIT_REPO "https://github.com/Cyan4973/xxHash")
FetchContent_Declare(
    xxhash
    GIT_REPOSITORY "https://github.com/Cyan4973/xxHash"
    GIT_TAG "f4bef929aa854e9f52a303c5e58fd52855a0ecfa"
    
)
message(STATUS "Loading xxhash - ${XXHASH_GIT_REPO}")
FetchContent_MakeAvailable(xxhash)