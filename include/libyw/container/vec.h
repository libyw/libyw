#ifndef LIBYW_CONTAINER_VEC_H
#define LIBYW_CONTAINER_VEC_H

#include "libyw/core/allocator.h"
#include "libyw/core/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void* data;
    size_t len;
    size_t capacity;
    size_t elem_size;
    yw_allocator* alloc;
} yw_vec;

YW_NODISCARD yw_result yw_vec_init(yw_vec* vec, size_t elem_size, yw_allocator* alloc);
void yw_vec_free(yw_vec* vec);

YW_NODISCARD yw_result yw_vec_reserve(yw_vec* vec, size_t new_cap);
YW_NODISCARD yw_result yw_vec_push_back(yw_vec* vec, const void* elem_ptr);
YW_NODISCARD void* yw_vec_get(const yw_vec* vec, size_t index);

#ifdef __cplusplus
}
#endif

#endif // LIBYW_CONTAINER_VEC_H