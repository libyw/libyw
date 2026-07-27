#ifndef LIBYW_STRING_H
#define LIBYW_STRING_H

#include "libyw/core/types.h"

typedef struct {
    const char* data;
    size_t len;
} yw_string_view;

YW_INLINE yw_string_view yw_sv_from_cstr(const char* str) {
    size_t len = 0;
    if (str != nullptr) {
        while (str[len] != '\0') len++;
    }
    return (yw_string_view){ .data = str, .len = len };
}

YW_INLINE yw_string_view yw_sv_from_raw(yw_string_view sv) {
    return sv;
}

YW_NODISCARD YW_INLINE yw_string_view yw_sv_from_parts(const char* str, size_t len) {
    return (yw_string_view){ .data = str, .len = len };
}

YW_NODISCARD bool yw_sv_equals(yw_string_view a, yw_string_view b);

YW_NODISCARD yw_result yw_sv_find_char(yw_string_view sv, char ch, size_t* out_index);

YW_NODISCARD yw_result yw_memcpy_safe(void* dest, size_t dest_capacity, const void* src, size_t count);

#endif // LIBYW_STRING_H
