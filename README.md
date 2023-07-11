# BufferFS
## FUSE filesystem for streaming purposes
We have an Splay tree for storing metadata of files and searching them. Files are described with `bfs_file` structure,
Splay tree stores files **by value**.

Each file contains three buffers - beginning, end, and movable one. Buffers are desribed with `bfs_buffer` structure. Buffers are accessed from file by pointer, and should be `malloc`ed manually.

On read or write, buffer calculates overlap of segment first, then reads or writes in that overlap, splitting operation into two if necessary. 