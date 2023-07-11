#include "bfs_buffer.hpp"
#include "RTree.hpp"
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <vector>
#include <set>


bool bfs_buffer_cmp (const bfs_buffer &a, const bfs_buffer &b) {
    return a.descriptor < b.descriptor;
}

/// For any file operation, mutex must be locked
struct bfs_file
{
    const uint64_t fh;
    const char *path_real_prefix; // prefix of the path in real file
    const char *const path; // path is relative
    size_t pretended_file_size;
    pthread_mutex_t mutex;
    
    std::set<bfs_buffer, decltype(bfs_buffer_cmp)> buffers; // buffers by descriptor
    std::set<std::pair<off_t, off_t>> intervals; // using interval technique described here: https://codeforces.com/blog/entry/57934?#comment-416182
    RTree<bfs_buffer *, off_t, 2> rtree; // buffers by off_t pairs
};


/// @brief Initializes fields of given `bfs_file` structure representing empty file.
/// Immediately locks file mutex, it should be unlocked by parent function.
/// File handle should be created by the calling function.
int bfs_file_init(bfs_file *file, const uint64_t fh, const char *const path);


/// @brief Create new buffer inside of file
/// @return Descriptor of new buffer
uint64_t bfs_file_new_buffer(bfs_file *file, size_t size, off_t offset);


/// @brief Returns pointer to buffer by its descriptor
bfs_buffer *bfs_file_get_buffer(bfs_file *file, uint64_t descriptor);


/// Slides buffer pointed by descriptor
void bfs_file_slide_buffer(bfs_file *file, uint64_t descriptor);


/// @brief Deletes buffer inside of file
/// @param buf_descriptor descriptor of buffer
void bfs_file_delete_buffer(bfs_file *file, uint64_t buf_descriptor);


/// @brief Writes BFS file
void bfs_file_write(struct bfs_file *file, const char *buf, size_t length, off_t offset);


/// @brief Reads BFS file
void bfs_file_read(struct bfs_file *file, const char *buf, size_t length, off_t offset);
