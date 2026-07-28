#ifndef LIBYW_SYS_STAT_H
#define LIBYW_SYS_STAT_H

#include <stdint.h>

#if defined(__x86_64__)
typedef struct yw_stat {
    uint64_t st_dev;
    uint64_t st_ino;
    uint64_t st_nlink;
    uint32_t st_mode;
    uint32_t st_uid;
    uint32_t st_gid;
    uint32_t __pad0;
    uint64_t st_rdev;
    int64_t  st_size;
    int64_t  st_blksize;
    int64_t  st_blocks;
    uint64_t st_atime;
    uint64_t st_atime_nsec;
    uint64_t st_mtime;
    uint64_t st_mtime_nsec;
    uint64_t st_ctime;
    uint64_t st_ctime_nsec;
    int64_t  __unused[3];
} yw_stat;

#elif defined(__aarch64__)
typedef struct yw_stat {
    uint64_t st_dev;
    uint64_t st_ino;
    uint32_t st_mode;
} yw_stat;
#else
#error "Unsupported architecture"
#endif

#endif
