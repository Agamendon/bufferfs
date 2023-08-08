#include <bfs_file.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct mmblock {
    off_t start;
    off_t end;
};

struct iter_data {
    bfs_buffer *buf;
    off_t start; // offsets that should be considered on IO
    off_t end; // above
//    off_t bstart; // start of buffer in the file
    FILE *stream;
};


struct iter_rw_data
{
    const char *rwbuf;
    off_t current_ptr;
};


/// start[2], end[2]
/// Returns intersection of two memory blocks
/// Assumes that they intersect
inline mmblock intersect_offsets(off_t *start, off_t *end)
{
    return (mmblock){ start[0] > start[1] ? start[0] : start[1],
    end[0] < end[1] ? end[0] : end[1] };
}


/// Finds specific buffer in RTree during deletion process
bool iter_find_buffer(bfs_buffer *min, bfs_buffer *max, uint64_t *descriptor)
{
    if (buffer->descriptor == descriptor) return false;
    return true;
}


/// On events that require writing, write from intersections
/// UData is a structure
bool iter_write_from_neighbors(const uint64_t *min, const uint64_t *max,
          void *data, void *udata)
{
    off_t start[2] = {(iter_data *)udata->start, (bfs_buffer *)data->offset};
    off_t end[2] = {(iter_data *)udata->end, (bfs_buffer *)data->offset \
        + (bfs_buffer *)data->size - 1};
    mmblock istn = intersect_offsets(start, end);
    bfs_buffer_copy_on_the_fly((bfs_buffer *data),(iter_data *)udata->buf,
                               istn.start, istn.start, istn.start - istn.end + 1,
                               (iter_data *)udata->stream, (iter_data *)udata->stream);
    return true;
}


/// One step in file reading operation
bool iter_write(const bfs_buffer *buf, iter_rw_data *udata)
{

}


/// Finds intersections of given buffer with other buffers of file,
/// executes iter() for each of them
void search_intersections(bfs_file *file, bfs_buffer *buffer,
            bool (*iter)(const uint64_t *min,
            const uint64_t *max, void *data,
            void *udata), void *udata)
{
    // start position is [0, offset + size - 1]
    // end position is [offset, infty]
    uint64_t min[2] = {0, buffer->offset};
    uint64_t max[2] = {buffer->offset + buffer->size - 1, UINT64_MAX};
    struct iter_data
    rtree_search(file->ftree, min, max, &iter_write_from_neighbors, &);
}


/// Return condition of set_walk(mset) on file creation
bool ret_allocate(struct *mmblock block, size_t *size)
{
    return (block->end - block->start + 1) >= size;
 }


// this is quite an unoptimal implementation, as data may get
// reduntantly overwritten several times. Add memory tracking later.
struct bfs_file *bfs_file_create(const char *prefix, const char *path
                            uint64_t descriptor)
{
    bfs_file *file = malloc(sizeof(bfs_file));
    if (!file) return NULL;
    file->descriptor = descriptor;
    strcpy(file->path, fpath); // init path
    file->psize = 0;
    file->fset = set_init(); // init set, value is bfs_buffer *
    file->ftree = rtree_new(); // init rtree

    file->mset = set_init(); // init set, value is mmblock *
    mmblock *block = malloc(sizeof(mmblock));
    block->start = NULL;
    block->end = UINTPTR_MAX;
    if (!block)
        return NULL;
    set_insert(mset, NULL, block);
    return file;
}


uint64_t bfs_file_add_buffer(bfs_file *file, size_t size)
{
    mmblock *block = (mmblock *)set_walk_until(file->mset, &ret_allocate, &size);
    block->end -= size;
    bfs_buffer *buf = malloc(sizeof(bfs_buffer));
    if (!buffer)
        return 0;
    uint64_t descriptor;
    do {
        descriptor = random();
    } while (set_find(file->fset, descriptor));
    if(bfs_buffer_init(buf, descriptor, 0, size)) return NULL;
    buf->start = block->start;
    if (block->end < block->start) {
        set_delete(file->fset, block->start);
        free(block);
    }
    struct iter_data iter_data  = {.buf = buf, .start = buf->start,
        .end = buf->start + buf->size - 1, file->stream};
    search_intersections(file, buf, &iter_write_from_neighbors, &iter_data);
    rtree_insert(file->ftree, {buf->offset, buf->offset + buf->size - 1}, {buf->offset, buf->offset + buf->size - 1},
                 buf); // let's assume the tree copies only the pointer to buffer
    return buf->descriptor;
}


int bfs_file_slide_buffer(bfs_file *file, uint64_t descriptor, size_t length)
{
    bfs_buffer *buf = set_find(bfs_file->fset, descriptor);
    if (!buf) return -1;
    struct iter_data iter_data  = {.buf = buf, file->stream};
    if (length > 0) {
            iter_data->start = buf->offset + buf->size;
            iter_data->end = buf->offset + buf->size + length - 1;
    } else if (length < 0) {
            iter_data->start = buf->offset - length;
            iter_data->end = buf->offset - 1;
    }
    if (bfs_buffer_slide(buf, length)) return -1;
    if (iter_data->end - iter_data->start >= buf->size) {
        iter_data->start = buf->start;
        iter_data->end = buf->start + buf->offset - 1;
    }
    search_intersections(file, buf, &iter_write_from_neighbors, &iter_data);
    return 0;
}


int bfs_file_remove_buffer(bfs_file *file, uint64_t descriptor)
{
    bfs_buffer *buf = set_find(file->fset, descriptor);
    rtree_delete_with_comparator(file->ftree, {buf->offset, buf->offset + buf->size - 1},
                 {buf->offset, buf->offset + buf->size - 1}, buf,
                 &iter_find_buffer, &descriptor);
    mmblock *block = set_lower_bound(file->mset, buf->offset);
    if (!block) {
        block = malloc(sizeof(mmblock));
        if (!block) return -1;
        block = { .start = buf->offset, .end = buf->offset + buf->size - 1 };
        set_insert(file->mset, buf->offset, &block);
    }
    mmblock *next = (mmblock *)set_next(file->mset, block->start);
    if (next->start == block->end + 1) {
        block->end = next->end;
        set_delete(file->mset, next->start);
    }
    rtree_delete_with_comparator(file->ftree,
                                 {buf->start, buf->start + buf->offset - 1},
                                 {buf->start, buf->start + buf->offset - 1},
                                 &buf, &iter_find_buffer, &descriptor);
    free(buf);
    return 0;
}


int bfs_file_read(bfs_file *file, char *to, off_t offset, size_t size)
{

}
