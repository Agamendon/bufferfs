#include "bfs_file.hpp"
#include <climits>


#define UNSIGNED_MAX_VAL(ot) ((1 << (sizeof(ot) * CHAR_BIT - 1) - 1) & (1 << (sizeof(ot) * CHAR_BIT - 1)))


int bfs_file_init(struct bfs_file *file, const uint64_t fh, const char *const path)
{
    pthread_mutex_lock(&bfs_file->mutex);
    uint64_t fh;
    do {
        fh = random();
    } while(!fh && file->s);
    file->path = strcpy(&bfs_file->path, path);
    file->pretended_file_size = 0;
    file->buffers_by_descriptor = btree_new(sizeof(bfs_file), 0, &bfs_file_compare, NULL);
    file->buffers_by_coords = rtree_new();
}


uint64_t bfs_file_new_buffer(struct bfs_file *file, size_t size, off_t offset)
{
    struct bfs_buffer to_insert = {
        .offset = 0;
        .size = 0;
    };

    do {
        to_insert.descriptor = random();
    } while(btree_get(file->buffers_by_descriptor, &to_insert) != NULL);
    btree_set(file->buffers_by_descriptor, &to_insert);

    // rtree insertion makes no sense with 0,0 - it will be done during first slide
    return to_insert.descriptor;
}


bfs_buffer *bfs_file_get_buffer(bfs_file *file, uint64_t descriptor)
{
    struct bfs_buffer to_search = {
            .descriptor = descriptor;
    };
    return btree_get(file->buffers_by_descriptor, &to_search);
}


void bfs_file_delete_buffer(bfs_file *file, uint64_t buf_descriptor)
{
    struct bfs_buffer to_delete = {
            .descriptor = descriptor;
    };
    bfs_buffer *deleted = btree_delete(file->buffers_by_descriptor, &to_delete);
    rtree_delete_with_comparator(file->buffers_by_coords, deleted->size, deleted->offset, deleted);
    return;
}


void bfs_file_write(struct bfs_file *file, const char *buf, size_t length, off_t offset)
{
    const off_t coords_min[2] = {0, offset + length};
    const off_t coords_max[2] = {offset, UNSIGNED_MAX_VAL(offset)};
    rtree_search(file->buffers_by_coords, coords_min, coords_max, 
}