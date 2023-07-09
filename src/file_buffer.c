#include <file_buffer.h>

//
// Static functions
//


static off_t max(off_t a, off_t b)
{
    if (a > b)
        return a;
    return b;
}


static off_t min(off_t a, off_t b)
{
    if (a > b)
        return b;
    return a;
}


static size_t normalize(size_t x, size_t min, size_t max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}


/// @brief Initialize "private" fields of file buffer.
/// Buffer should be initialized before this function is called.
static void file_buffer_private_init(file_buffer *fbuf)
{
    fbuf->infile_offset = 0;
    fbuf->infile_accessible_offset = 0;
    fbuf->infile_accessible_size = 0;
    mtx_init(&fbuf->buffer_mtx, mtx_plain);
}


//
// Library functions
//


void file_buffer_init(file_buffer *fbuf, off_t offset, size_t size, unsigned descriptor, char *base_directory)
{
    fbuf->offset = offset;
    fbuf->size = size;
    fbuf->descriptor = descriptor;
    fbuf->base_directory = base_directory;

    file_buffer_private_init(fbuf);
}


void file_buffer_slide(file_buffer *fbuf, off_t distance)
{
    mtx_lock(&fbuf->buffer_mtx);
    fbuf->offset += distance; 
    fbuf->infile_offset = (fbuf->offset + fbuf->size + distance) % fbuf->size;
    fbuf->zeros_start = normalize(fbuf->zeros_start - distance,  0, fbuf->size);
    fbuf->zeros_end = normalize(fbuf->zeros_end + distance, 0, fbuf->size);
    mtx_unlock(&fbuf->buffer_mtx);
}


void file_buffer_move(file_buffer *fbuf, off_t offset)
{
    file_buffer_slide(fbuf, offset - fbuf->offset);
}