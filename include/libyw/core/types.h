#ifndef LIBYW_CORE_TYPES_H
#define LIBYW_CORE_TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define YW_NODISCARD [[nodiscard]]
#define YW_INLINE static inline

typedef enum : int32_t {
    YW_SUCCESS = 0,
    YW_ERR_INVALID_ARGUMENT = -1,
    YW_ERR_OUT_OF_BOUNDS   = -2,
    YW_ERR_NO_MEMORY       = -3,
    YW_ERR_NOT_FOUND       = -4
} yw_result;

typedef struct {
    void* value;
    bool has_value;
} yw_option_ptr;

YW_INLINE yw_option_ptr yw_some_ptr(void* val) {
    return (yw_option_ptr){ .value = val, .has_value = true };
}

YW_INLINE yw_option_ptr yw_none_ptr(void) {
    return (yw_option_ptr){ .value = nullptr, .has_value = false };
}

#endif // LIBYW_CORE_TYPES_H