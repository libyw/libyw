#ifndef LIBYW_SYS_LINUX_H
#define LIBYW_SYS_LINUX_H

#include "libyw/core/types.h"

#define YW_SYS_MMAP   9
#define YW_SYS_MUNMAP 11

#define YW_PROT_READ     0x1
#define YW_PROT_WRITE    0x2
#define YW_MAP_PRIVATE   0x02
#define YW_MAP_ANONYMOUS 0x20

YW_INLINE void* yw_sys_mmap(void* addr, size_t length, int prot, int flags, int fd, int64_t offset) {
    long ret;
    register long r10 __asm__("r10") = flags;
    register long r8  __asm__("r8")  = fd;
    register long r9  __asm__("r9")  = offset;

    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(YW_SYS_MMAP), "D"(addr), "S"(length), "d"(prot), "r"(r10), "r"(r8), "r"(r9)
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

#endif // LIBYW_SYS_LINUX_H