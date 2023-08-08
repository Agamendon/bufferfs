#include <stdint.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

struct bfs_buffer {
    uint64_t descriptor;
    off_t offset;
    size_t size;
    off_t start; // start in file representation
};


/// Sets buffer values.
/// Returns NULL on success
void bfs_buffer_init(struct bfs_buffer *buffer, uint64_t descriptor, off_t offset, size_t size, off_t start);


// Returns 0 on success
/// @param start indicates position in file at which buffer representation starts
/// @param offset absolute offset in the file
/// It is assumed that offset and size are normalized
int bfs_buffer_write(struct bfs_buffer *buffer, const char *from, size_t size, off_t offset, FILE *stream);


// Returns 0 on  success
/// @param start indicates position in file at which buffer representation starts
/// @param offset absolute offset in the file
int bfs_buffer_read(struct bfs_buffer *buffer, const char *to, size_t size, FILE *stream);


/// Returns 0 on success
int bfs_buffer_copy_on_the_fly(bfs_buffer *bfrom, bfs_buffer *bto,
                               off_t offset_from, off_t offset_to,
                               size_t size, FILE *stream_from,
                               FILE *stream_to);


/// Slides buffer
/// Returns 0 on success, EFAULT if resulting offset
/// is less than 0. Buffer offset is not modified in case of error.
/// @param start indicates position in file at which buffer representation starts
int bfs_buffer_slide(struct bfs_buffer *buffer, long length);


