#include "libyw/core/allocator.h"

#if defined(YW_FREESTANDING)
yw_allocator* yw_allocator_system(void) {
    return nullptr;
}
#else

#include "sys/sys_linux.h"

static void* system_alloc_impl(yw_allocator* self, size_t size, size_t alignment) {
    (void)self;
    (void)alignment;

    if (size == 0) return nullptr;

    size_t total_size = size + sizeof(size_t);
    
    void* ptr = yw_sys_mmap(
        nullptr, 
        total_size, 
        YW_PROT_READ | YW_PROT_WRITE, 
        YW_MAP_PRIVATE | YW_MAP_ANONYMOUS, 
        -1, 
        0
    );

    if (ptr == nullptr) return nullptr;

    *(size_t*)ptr = total_size;
    return (void*)((unsigned char*)ptr + sizeof(size_t));
}

static void system_dealloc_impl(yw_allocator* self, void* ptr, size_t size) {
    (void)self;
    (void)size;
    if (ptr == nullptr) return;

    void* real_ptr = (unsigned char*)ptr - sizeof(size_t);
    size_t total_size = *(size_t*)real_ptr;

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
