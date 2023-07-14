#include <stdio.h>
#include <bfs_buffer.h>
#include <stdlib.h>



int main()
{
    struct bfs_buffer buffer;
    const uint32_t buffer_descriptor = 1;
    const off_t buffer_offset = 8;
    const size_t buffer_size = 8;
    const uint32_t buffer_chunk = 0;
    bfs_buffer_init(&buffer, buffer_descriptor, buffer_offset,
                    buffer_size, buffer_chunk);
    char *from = "123456789012345678901234567890";
    FILE *stream = fopen("/home/heorhii/bufferfs/test.txt", "r+");
    printf("%d\n", bfs_buffer_write(&buffer, from, 8, 0, stream, 10));
    if(bfs_buffer_slide(&buffer, -5, stream, 10) != 0)
        printf("slide error\n");
    printf("Test completed\n");
    return 0;
}
