#include <set.h>
#include <rtree.h>
#include <stdint.h>
#include <bfs_buffer.h>
#include <pthread.h>


struct bfs_file {
    const char path[256]; // real location of file. Normal access would be by handle.
    uint64_t descriptor;
    pthread_mutex_t *mute
    size_t psize; // pretended file size, as if all write operations were real
    struct set *fset; // buffers by descriptors
    struct rtree *ftree; // buffers by offsets of beginnign and end
    struct set *mset; // contains free storage intervals - mmblock structure
    FILE *stream;
};


/// Returns NULL on error
/// Note: prefix and path should be null-terminated
struct bfs_file *bfs_file_create(const char *prefix, const char *path,
                            uint64_t descriptor);


/// @returns Buffer descriptor
uint64_t bfs_file_add_buffer(bfs_file *file, size_t size);


/// @param descriptor buffer descriptors
/// Returns zero on success
int bfs_file_slide_buffer(bfs_file *file, uint64_t descriptor, size_t length);


/// @param descriptor buffer descriptor
/// Returns zer oon success
int bfs_file_remove_buffer(bfs_file *file, uint64_t descriptor);


/// Returns zero on success
int bfs_file_read(bfs_file *file, char *to, off_t offset, size_t size);


/// Returns zero on success
int bfs_file_write(bfs_file *file, const char *from, off_t offset, size_t size);


int bfs_file_delete(bfs_file *file);
