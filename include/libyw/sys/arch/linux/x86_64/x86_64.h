/*
 * : x86                                :
 * : 64-bit Linux system call interface :
 * : ABI: x86_64                        :
 */

#ifndef LIBYW_SYS_LINUX_X86_64_H
#define LIBYW_SYS_LINUX_X86_64_H

#include "libyw/core/types.h"
#include "libyw/sys/stat.h"

// System Call Numbers
#define YW_SYS_READ                0
#define YW_SYS_WRITE               1
#define YW_SYS_OPEN                2
#define YW_SYS_CLOSE               3
#define YW_SYS_STAT                4
#define YW_SYS_MMAP                9
#define YW_SYS_MUNMAP             11
#define YW_SYS_EXIT               60
#define YW_SYS_KILL               62
#define YW_SYS_MKDIR              83
#define YW_SYS_RMDIR              84
#define YW_SYS_CHMOD              90
#define YW_SYS_CLOCK_GETTIME     228

// Flags & Constants
// mmap flags
#define YW_PROT_READ     0x1
#define YW_PROT_WRITE    0x2
#define YW_MAP_PRIVATE   0x02
#define YW_MAP_ANONYMOUS 0x20

// open flags (Linux x86_64)
#define YW_O_RDONLY  00000000
#define YW_O_WRONLY  00000001
#define YW_O_RDWR    00000002
#define YW_O_CREAT   00000100
#define YW_O_TRUNC   00001000
#define YW_O_APPEND  00002000

// System Call Functions
YW_INLINE int64_t yw_sys_read(unsigned int fd, char* buf, size_t count) {
    long ret;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(YW_SYS_READ), "D"((long)fd), "S"(buf), "d"(count)
        : "rcx", "r11", "memory"
    );
    return ret;
}

YW_INLINE int64_t yw_sys_write(unsigned int fd, const char* buf, size_t count) {
    long ret;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(YW_SYS_WRITE), "D"((long)fd), "S"(buf), "d"(count)
        : "rcx", "r11", "memory"
    );
    return ret;
}

YW_INLINE int yw_sys_open(const char* filename, int flags, yw_mode_t mode) {
    long ret;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(YW_SYS_OPEN), "D"(filename), "S"(flags), "d"(mode)
        : "rcx", "r11", "memory"
    );
    return (int)ret;
}

YW_INLINE int yw_sys_close(unsigned int fd) {
    long ret;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(YW_SYS_CLOSE), "D"((long)fd)
        : "rcx", "r11", "memory"
    );
    return (int)ret;
}

YW_INLINE int64_t yw_sys_stat(const char* filename, yw_stat* statbuf) {
    long ret;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(YW_SYS_STAT), "D"(filename), "S"(statbuf)
        : "rcx", "r11", "memory"
    );
    return ret;
}

YW_INLINE void* yw_sys_mmap(void* addr, unsigned long len, int64_t prot, unsigned long flags, int64_t fd, int64_t off) {
    long ret;
    YW_REGISTER long r10 __asm__("r10") = (long)flags;
    YW_REGISTER long r8  __asm__("r8")  = (long)fd;
    YW_REGISTER long r9  __asm__("r9")  = (long)off;

    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(YW_SYS_MMAP), "D"((unsigned long)(uintptr_t)addr), "S"(len), "d"((unsigned long)prot), "r"(r10), "r"(r8), "r"(r9)
        : "rcx", "r11", "memory"
    );

    if (ret < 0 && ret > -4096) {
        return nullptr;
    }
    return (void*)ret;
}

YW_INLINE int yw_sys_munmap(void* addr, size_t length) {
    long ret;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(YW_SYS_MUNMAP), "D"(addr), "S"(length)
        : "rcx", "r11", "memory"
    );
    return (int)ret;
}

YW_NORETURN
YW_INLINE void yw_sys_exit(int status) {
    __asm__ __volatile__(
        "syscall"
        :
        : "a"(YW_SYS_EXIT), "D"((long)status)
        : "rcx", "r11", "memory"
    );
    __builtin_unreachable();
}

YW_INLINE int yw_sys_kill(int pid, int sig) {
    long ret;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(YW_SYS_KILL), "D"(pid), "S"(sig)
        : "rcx", "r11", "memory"
    );
    return (int)ret;
}

YW_INLINE int yw_sys_mkdir(const char* pathname, yw_mode_t mode) {
    long ret;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(YW_SYS_MKDIR), "D"(pathname), "S"(mode)
        : "rcx", "r11", "memory"
    );
    return (int)ret;
}

YW_INLINE int yw_sys_rmdir(const char *pathname) {
    long ret;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(YW_SYS_RMDIR), "D"(pathname)
        : "rcx", "r11", "memory"
    );
    return (int)ret;
}

typedef struct {
    long tv_sec;
    long tv_nsec;
} yw_timespec_t;


YW_INLINE int yw_sys_gettime(const int which_clock, yw_timespec_t *tp) {
    long ret;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(YW_SYS_CLOCK_GETTIME), "D"((long)which_clock), "S"(tp)
        : "rcx", "r11", "memory"
    );
    return (int)ret;
}

YW_INLINE int yw_sys_chmod(const char *filename, yw_mode_t mode) {
    long ret;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(YW_SYS_CHMOD), "D"(filename), "S"(mode)
        : "rcx", "r11", "memory"
    );
    return (int)ret;
}

#endif // LIBYW_SYS_LINUX_X86_64_H
