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



void bfs_buffer_init(struct bfs_buffer *buffer, uint64_t descriptor, off_t offset, size_t size, off_t start)
{
    buffer->descriptor = descriptor;
    buffer->offset = offset;
    buffer->size = size;
    buffer->start = start;
    return;
}


int bfs_buffer_write(struct bfs_buffer *buffer, const char *from, size_t size, off_t offset, FILE *stream)
{
    // these ifs do bound checking
    if (offset + size - 1 < buffer->offset) return 0;
    if (offset >= buffer->offset + size  - 1) return 0;
    if (offset + size > buffer->offset + buffer->size) size -= (offset + size) - (buffer->offset + buffer->size);
    if (offset < buffer->offset) {
        size -= buffer->offset - offset;
        offset = buffer->offset;
    }


    off_t loop_start = offset % buffer->size;
    size_t loop_first = buffer->size - loop_start;
    size_t to_write = (size < loop_first) ? size : loop_first;

    int ret = fseek(stream, buffer->start + loop_start, SEEK_SET) ;
    if (ret) return ret;
    ret = fwrite(from, 1, to_write, stream);
    if (ret != to_write) return 1;

    if (size > loop_first) {
        to_write = size - loop_first;
        ret = fseek(stream, buffer->start, SEEK_SET);
        if (ret)
            return ret;
        ret = fwrite(from + loop_first, 1, to_write, stream);
        if (ret != to_write)
            return 1;
    }
    fflush(stream);
    return 0;
}


int bfs_buffer_read(struct bfs_buffer *buffer, const char *to, size_t size, off_t offset, FILE *stream)
{
    // these ifs do bound checking
    if (offset + size - 1 < buffer->offset) return 0;
    if (offset >= buffer->offset + size  - 1) return 0;
    if (offset + size > buffer->offset + buffer->size) size -= (offset + size) - (buffer->offset + buffer->size);
    if (offset < buffer->offset) {
        size -= buffer->offset - offset;
        offset = buffer->offset;
    }

    off_t loop_start = offset % buffer->size;
    size_t loop_first = buffer->size - loop_start;
    size_t to_read= (size < loop_first) ? size : loop_first;

    int ret = fseek(stream, buffer->start + loop_start, SEEK_SET) ;
    if (ret) return ret;
    ret = fread(to, 1, to_read, stream);
    if (ret != to_read) return 1;

//    printf("size: %d, to_read: %d\n", size, to_read);

    if (size > loop_first) {
        to_read = size - loop_first;
//        printf("size: %d, to_read: %d\n", size, to_read);
        ret = fseek(stream, buffer->start, SEEK_SET);
        if (ret) return ret;
        ret = fread(to + loop_first, 1, to_read, stream);
        if (ret != to_read) return 1;
    }
    fflush(stream);
    return 0;
}


int bfs_buffer_copy_on_the_fly(bfs_buffer *bfrom, bfs_buffer *bto,
                               off_t offset_from, off_t offset_to,
                               size_t size, FILE *stream_from,
                               FILE *stream_to)
{
    if (offset_from < bfrom->offset) { // normalizing offsets
        size -= bfrom->offset - offset_from;
        offset_to += bfrom->offset - offset_from;
        offset_from = bfrom->offset;
    } else if (offset_to < bto->offset) {
        size -= bto->offset - offset_to;
        offset_from += bto->offset - offset_to;
        offset_to = bto->offset;
    }
    if (offset_from + size > bfrom->offset + bfrom->size)
        size = bfrom->offset + bfrom->size - offset_from;
    if (offset_to+ size > bto>offset + bto->size)
        size = bto->offset + bto->size - offset_to;

    if (size <= 0) return 0;
    char c;
    if(fseek(stream_from, bfrom->start + offset_from % bfrom->size, SEEK_SET))
        return -1;

    if(fseek(stream_to, bto->start + offset_to % bto->size, SEEK_SET))
        return -1;
    for (size_t sz = 0; sz < size; sz++) {
            c = fgetc(stream_from);
            fputc(c, stream_to);
    }
    return 0;
}


int bfs_buffer_slide(struct bfs_buffer *buffer, long length)
{
    if (length == 0)
        return 0;
    if (buffer->offset + length < 0)
        return EFAULT;
    buffer->offset += length;
    return 0;
}
