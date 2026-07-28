#include "libyw/core/allocator.h"

#if defined(YW_FREESTANDING)
yw_allocator* yw_allocator_system(void) {
    return nullptr;
}
#else

#include "libyw/sys/sys.h"

YW_INLINE size_t align_up(size_t size, size_t align) {
    return (size + align - 1) & ~(align - 1);
}

static void* system_alloc_impl(yw_allocator* self, size_t size, size_t alignment) {
    (void)self;
    if (size == 0) return nullptr;

    if (alignment < 16) {
        alignment = 16;
    }

    size_t header_size = align_up(sizeof(size_t), alignment);
    size_t total_size  = size + header_size;

    void* ptr = yw_sys_mmap(
        nullptr, 
        total_size, 
        YW_PROT_READ | YW_PROT_WRITE, 
        YW_MAP_PRIVATE | YW_MAP_ANONYMOUS, 
        -1, 
        0
    );

    if (ptr == nullptr) return nullptr;

    void* user_ptr = (void*)((unsigned char*)ptr + header_size);
    *((size_t*)user_ptr - 1) = total_size;
    *((size_t*)user_ptr - 2) = header_size;

    return user_ptr;
}

static void system_dealloc_impl(yw_allocator* self, void* ptr, size_t size) {
    (void)self;
    (void)size;
    if (ptr == nullptr) return;

    size_t total_size  = *((size_t*)ptr - 1);
    size_t header_size = *((size_t*)ptr - 2);

    void* real_ptr = (unsigned char*)ptr - header_size;

    yw_sys_munmap(real_ptr, total_size);
}

static yw_allocator g_system_allocator = {
    .allocate   = system_alloc_impl,
    .deallocate = system_dealloc_impl,
    .ctx        = nullptr
};

yw_allocator* yw_allocator_system(void) {
    return &g_system_allocator;
}

void* yw_alloc(yw_allocator* alloc, size_t size, size_t alignment) {
    if (alloc == nullptr || alloc->allocate == nullptr) {
        return nullptr;
    }
    return alloc->allocate(alloc, size, alignment);
}

void yw_dealloc(yw_allocator* alloc, void* ptr, size_t size) {
    if (alloc == nullptr || alloc->deallocate == nullptr || ptr == nullptr) {
        return;
    }
    alloc->deallocate(alloc, ptr, size);
}

#endif
