#include "libyw/container/vec.h"
#include "libyw/string.h"

yw_result yw_vec_init(yw_vec* vec, size_t elem_size, yw_allocator* alloc) {
    if (vec == nullptr || elem_size == 0) {
        return YW_ERR_INVALID_ARGUMENT;
    }
    
    vec->data = nullptr;
    vec->len = 0;
    vec->capacity = 0;
    vec->elem_size = elem_size;
    vec->alloc = (alloc != nullptr) ? alloc : yw_allocator_system();

    return YW_SUCCESS;
}

void yw_vec_free(yw_vec* vec) {
    if (vec == nullptr) return;
    if (vec->data != nullptr && vec->alloc != nullptr) {
        yw_dealloc(vec->alloc, vec->data, vec->capacity * vec->elem_size);
    }
    vec->data = nullptr;
    vec->len = 0;
    vec->capacity = 0;
}

yw_result yw_vec_reserve(yw_vec* vec, size_t new_cap) {
    if (vec == nullptr) return YW_ERR_INVALID_ARGUMENT;
    if (new_cap <= vec->capacity) return YW_SUCCESS;

    size_t old_bytes = vec->capacity * vec->elem_size;
    size_t new_bytes = new_cap * vec->elem_size;

    void* new_data = yw_alloc(vec->alloc, new_bytes, _Alignof(max_align_t));
    if (new_data == nullptr) {
        return YW_ERR_NO_MEMORY;
    }

    if (vec->data != nullptr) {
        yw_result res = yw_memcpy_safe(new_data, new_bytes, vec->data, old_bytes);
        if (res != YW_SUCCESS) {
            yw_dealloc(vec->alloc, new_data, new_bytes);
            return res;
        }
        yw_dealloc(vec->alloc, vec->data, old_bytes);
    }

    vec->data = new_data;
    vec->capacity = new_cap;
    return YW_SUCCESS;
}

yw_result yw_vec_push_back(yw_vec* vec, const void* elem_ptr) {
    if (vec == nullptr || elem_ptr == nullptr) {
        return YW_ERR_INVALID_ARGUMENT;
    }

    if (vec->len >= vec->capacity) {
        size_t next_cap = (vec->capacity == 0) ? 4 : vec->capacity * 2;
        yw_result res = yw_vec_reserve(vec, next_cap);
        if (res != YW_SUCCESS) return res;
    }

    unsigned char* target = (unsigned char*)vec->data + (vec->len * vec->elem_size);
    yw_result res = yw_memcpy_safe(target, vec->elem_size, elem_ptr, vec->elem_size);
    if (res != YW_SUCCESS) return res;

    vec->len++;
    return YW_SUCCESS;
}

void* yw_vec_get(const yw_vec* vec, size_t index) {
    if (vec == nullptr || index >= vec->len || vec->data == nullptr) {
        return nullptr;
    }
    return (unsigned char*)vec->data + (index * vec->elem_size);
}
