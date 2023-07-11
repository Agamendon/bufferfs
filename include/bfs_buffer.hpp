#include <utility>
#include <stdlib.h>
#include <stdint.h>



struct bfs_buffer {
    uint64_t descriptor; // descriptor of buffer for usage in splay tree
    off_t offset; // offset in real file
    const size_t size; // size in real file
    const uint32_t pos; // position in storage file, starting with 0
};

/// Buffer sliding should be implemented by parent functions, as buffer does not have direct access to its storage

/// @brief Checks whether there will be division in two chunks during read/write
/// offset and size should be normalized by the calling function to fit in buffer fully, otherwise behaviour is unexpected
bool bfs_buffer_is_two_chunks(bfs_buffer *buffer, off_t offset, size_t size);


/// @brief This function returns beginning and offset of the first chunk in virtual buffer storage.
/// It does not regard possible offset of storage file.
/// It is implied that `bfs_buffer_is_two_chunks` returned `true` before
std::pair<off_t, off_t> bfs_buffer_get_first_chunk(bfs_buffer *buffer, off_t offset, size_t size);


/// @brief This function returns beginning and offset of the second chunk in virtual buffer storage.
/// It does not regard possible offset of storage file
std::pair<off_t, off_t> bfs_buffer_get_second_chunk(bfs_buffer *buffer, off_t offset, size_t size);