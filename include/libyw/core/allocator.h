#ifndef LIBYW_CORE_ALLOCATOR_H
#define LIBYW_CORE_ALLOCATOR_H

#include "libyw/core/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct yw_allocator yw_allocator;

typedef void* (*yw_alloc_fn)(yw_allocator* self, size_t size, size_t alignment);
typedef void  (*yw_free_fn)(yw_allocator* self, void* ptr, size_t size);

struct yw_allocator {
    yw_alloc_fn allocate;
    yw_free_fn  deallocate;
    void* ctx;
};

YW_NODISCARD void* yw_alloc(yw_allocator* alloc, size_t size, size_t alignment);

void yw_dealloc(yw_allocator* alloc, void* ptr, size_t size);

YW_NODISCARD yw_allocator* yw_allocator_system(void);

#ifdef __cplusplus
}
#endif

#endif // LIBYW_CORE_ALLOCATOR_H
