#include <bfs_buffer.h>
#include <stdlib.h>
#include <errno.h>


int fill_zeros(FILE *stream, size_t size, off_t offset)
{
    int ret = fseek(stream, offset, SEEK_SET);
    for(int i = offset; i < offset + size; i++) {
    if (ret) return ret;
        if(fputc('\0', stream) == EOF) return EOF;
    }
    return 0;
}



void bfs_buffer_init(struct bfs_buffer *buffer, uint32_t descriptor, off_t offset, size_t size, uint32_t chunk)
{
    buffer->descriptor = descriptor;
    buffer->offset = offset;
    buffer->size = size;
    buffer-> chunk = chunk;
    return;
}


int bfs_buffer_write(struct bfs_buffer *buffer, const char *from, size_t size, off_t offset, FILE *stream, off_t start)
{
    off_t loop_start = (buffer->offset + offset) % buffer->size;
    size_t loop_first = buffer->size - loop_start;
    size_t to_write = (size < loop_first) ? size : loop_first;

    int ret = fseek(stream, start + loop_start, SEEK_SET) ;
    if (ret) return ret;
    ret = fwrite(from, 1, to_write, stream);
    if (ret != to_write) return 1;

    if (size > loop_first) {
        to_write = size - loop_first;
        ret = fseek(stream, start, SEEK_SET);
        if (ret)
            return ret;
        ret = fwrite(from + loop_first, 1, to_write, stream);
        if (ret != to_write)
            return 1;
    }
    return 0;
}


int bfs_buffer_read(struct bfs_buffer *buffer, const char *to, size_t size, FILE *stream, off_t start)
{
    off_t loop_start = buffer->offset % buffer->size;
    size_t loop_first = buffer->size - loop_start;
    size_t to_read= (size < loop_first) ? size : loop_first;

    int ret = fseek(stream, start + loop_start, SEEK_SET) ;
    if (ret) return ret;
    ret = fread(to, 1, to_read, stream);
    if (ret != to_read) return 1;

//    printf("size: %d, to_read: %d\n", size, to_read);

    if (size > loop_first) {
        to_read = size - loop_first;
//        printf("size: %d, to_read: %d\n", size, to_read);
        ret = fseek(stream, start, SEEK_SET);
        if (ret) return ret;
        ret = fread(to + loop_first, 1, to_read, stream);
        if (ret != to_read) return 1;
    }
    return 0;
}


int bfs_buffer_slide(struct bfs_buffer *buffer, long length, FILE *stream, off_t start)
{
    if (length == 0)
        return 0;
    if (buffer->offset + length < 0)
        return EFAULT;
    if (abs(length) >= buffer->size) {
        if (fill_zeros(stream, buffer->size, start) == EOF)
            return EOF;
        else
            buffer->offset = (buffer->offset + buffer->size + length % buffer->size) % buffer->size;
    }

    off_t offset_new = (buffer->offset + buffer->size + length % buffer->size) % buffer->size;
    off_t loop_start;
    if (length > 0)
        loop_start = buffer->offset;
    else {
        loop_start = offset_new;
        length = -length;
    }
    size_t loop_first = buffer->size - loop_start;
    size_t to_write = (length < loop_first) ? length : loop_first;
    if (fill_zeros(stream, to_write, start + loop_start) == EOF)
        return EOF;
    if (length > loop_first) {
        to_write = length - loop_first;
        if (fill_zeros(stream, to_write, start) == EOF)
            return EOF;
    }
    buffer->offset = offset_new;
    return 0;
}
