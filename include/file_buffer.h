#include <stdlib.h>
#include <threads.h>


/// @brief Struct that represents file buffer in a particular file.
/// @param offset               buffer start offset
/// @param size                 buffer size, bytes. Should not be modified.
typedef struct file_buffer
{
    off_t offset;
    size_t size;

    // "private" values, don't modify externally
    // the real file in which buffer is stored is cyclical
    // 
    off_t accessible_offset; // offset of accessible part of file
    size_t accesible_size; // length of accessible part of file
    mtx_t buffer_mtx; // mutex for file access
} file_buffer;


/// @brief Initialize a new file buffer object.
/// @param fbuf                 pointer to file buffer
/// @param offset               buffer start offset in real file, bytes
/// @param length               buffer length, bytes
/// @param descriptor           unique (for one file) buffer descriptor
/// @warning Base directory string is not copied by value, but by reference.
void file_buffer_init(file_buffer *fbuf, off_t offset, size_t size);


/// @brief Slides the buffer.
/// @param fbuf                 pointer to file buffer
/// @param distance             slide distance, bytes
void file_buffer_slide(file_buffer *fbuf, off_t distance);


/// @brief Moves buffer to specified position
/// @param fbuf                 pointer to file buffer
/// @param start                new buffer offset
void file_buffer_move(file_buffer *fbuf, off_t offset);


/// @brief Write bytes from memory buffer `buf` to file buffer.
/// @param fbuf                 pointer to file buffer
/// @param buf                  pointer to writing source
/// @param size                 number to bytes to read from byte buffer
/// @param offset               offset in @b real @b file
/// @return Number of bytes actually written to file buffer.
size_t file_buffer_write(file_buffer *fbuf, char *buf, size_t size, off_t offset);


/// @brief Read bytes to memory buffer `buf` from file buffer.
/// @param fbuf                 pointer to file buffer
/// @param buf                  pointer to reading destination
/// @param size                 number of bytes to write to byte buffer
/// @param offset               offset in @b real @b file
/// @return Number of bytes actually read from file buffer.
size_t file_buffer_read(file_buffer *fbuf, char *buf, size_t size, off_t offset);