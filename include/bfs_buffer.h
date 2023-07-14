#include <stdint.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

struct bfs_buffer {
    uint32_t descriptor;
    off_t offset;
    size_t size;
    uint32_t chunk; // number of chunk in file representation
};


/// Sets buffer values.
/// Returns NULL on success
void bfs_buffer_init(struct bfs_buffer *buffer, uint32_t descriptor, off_t offset, size_t size, uint32_t chunk);


// Returns 0 on success
/// @param start indicates position in file at which buffer representation starts
/// It is assumed that offset and size are normalized
int bfs_buffer_write(struct bfs_buffer *buffer, const char *from, size_t size, off_t offset, FILE *stream, off_t start);


// Returns 0 on  success
/// @param start indicates position in file at which buffer representation starts
int bfs_buffer_read(struct bfs_buffer *buffer, const char *to, size_t size, FILE *stream, off_t start);


/// Slides buffer, fills unknown bytes with zeros
/// Returns 0 on success, EOF on IO error or EFAULT if resulting offset
/// is less than 0. Buffer offset is not modified in case of error.
/// @param start indicates position in file at which buffer representation starts
int bfs_buffer_slide(struct bfs_buffer *buffer, long length, FILE *stream, off_t start);


