#include <bfs_buffer.hpp>
#include <string.h>


typedef struct pair_off_t {
    off_t first;
    off_t second;
} pair_off_t;


bool bfs_buffer_is_two_chunks(bfs_buffer *buffer, off_t offset, size_t size)
{
    off_t first_pos = buffer->offset % buffer->size;
    if (first_pos + (offset - buffer->offset) >= buffer->size)
        return false;
    else
        return true;
}

pair_off_t bfs_buffer_get_first_chunk(bfs_buffer *buffer, off_t offset, size_t size)
{
    off_t first_pos = buffer->offset % buffer->size;
    return pair_off_t { .first = first_pos, .second = buffer->size - first_pos + 1 };
}


pair_off_t bfs_buffer_get_second_chunk(bfs_buffer *buffer, off_t offset, size_t size)
{
    off_t first_pos = buffer->offset % buffer->size;
    return pair_off_t { .first = 0, .second = size - (buffer->size - first_pos)};
}


