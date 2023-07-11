#include <fuse.h>


void *bfs_init(struct fuse_conn_info *conn);


void bfs_destroy(void *private_data);


int bfs_getattr(const char* path, struct stat* stbuf);


int bfs_fgetattr(const char* path, struct stat* stbuf);


int bfs_access(const char* path, int mask);


int bfs_readlink(const char* path, char* buf, size_t size);


int bfs_opendir(const char* path, struct fuse_file_info* fi);


int bfs_readdir(const char* path, void* buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* fi);


int bfs_mknod(const char* path, mode_t mode, dev_t rdev);


int bfs_mkdir(const char* path, mode_t mode);


int bfs_unlink(const char* path);


int bfs_rmdir(const char* path);


int bfs_symlink(const char* to, const char* from);


int bfs_rename(const char* from, const char* to);


int bfs_link(const char* from, const char* to);


int bfs_chmod(const char* path, mode_t mode);


int bfs_chown(const char* path, uid_t uid, gid_t gid, struct fuse_file_info *fi);


int bfs_truncate(const char* path, off_t size);


int bfs_ftruncate(const char* path, off_t size);


int bfs_utimens(const char *, const struct timespec tv[2], struct fuse_file_info *fi);


int bfs_open(const char* path, struct fuse_file_info* fi);


int bfs_read(const char* path, char *buf, size_t size, off_t offset, struct fuse_file_info* fi);


int bfs_write(const char* path, char *buf, size_t size, off_t offset, struct fuse_file_info* fi);


int bfs_statfs(const char *, struct statvfs *);


int bfs_release(const char* path, struct fuse_file_info *fi);


int bfs_releasedir(const char* path, struct fuse_file_info *fi);


int bfs_fsync(const char* path, int isdatasync, struct fuse_file_info* fi);


int bfs_fsyncdir(const char* path, int isdatasync, struct fuse_file_info* fi);


int bfs_flush(const char* path, struct fuse_file_info* fi);


int bfs_lock(const char* path, struct fuse_file_info* fi, int cmd, struct flock* locks);


int bfs_bmap(const char* path, size_t blocksize, uint64_t* blockno);


int bfs_setxattr(const char* path, const char* name, const char* value, size_t size, int flags);


int bfs_getxattr(const char* path, const char* name, char* value, size_t size);


int bfs_listxattr(const char* path, const char* list, size_t size);


int bfs_ioctl(const char *, unsigned int cmd, void *arg, struct fuse_file_info *, unsigned int flags, void *data);


int bfs_poll(const char* path, struct fuse_file_info* fi, struct fuse_pollhandle* ph, unsigned* reventsp);


int bfs_write_buf(const char *, struct fuse_bufvec *buf, off_t off, struct fuse_file_info *);


int bfs_read_buf(const char *, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *);


/// Can be unimplemented
int bfs_flock(const char *, struct fuse_file_info *, int op);


int fallocate(const char *, int, off_t, off_t, struct fuse_file_info *);


/// Can be unimplemented
ssize_t bfs_copy_file_range(const char *path_in, struct fuse_file_info *fi_in, off_t offset_in, const char *path_out,
struct fuse_file_info *fi_out, off_t offset_out, size_t size, int flags);


off_t bfs_lseek(const char *, off_t off, int whence, struct fuse_file_info *);