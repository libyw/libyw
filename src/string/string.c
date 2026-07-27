#include "libyw/string.h"

bool yw_sv_equals(yw_string_view a, yw_string_view b) {
    if (a.len != b.len) {
        return false;
    }
    if (a.data == b.data) {
        return true;
    }
    if (a.data == nullptr || b.data == nullptr) {
        return false;
    }

    for (size_t i = 0; i < a.len; ++i) {
        if (a.data[i] != b.data[i]) {
            return false;
        }
    }
    return true;
}

yw_result yw_sv_find_char(yw_string_view sv, char ch, size_t* out_index) {
    if (out_index == nullptr) {
        return YW_ERR_INVALID_ARGUMENT;
    }
    if (sv.data == nullptr && sv.len > 0) {
        return YW_ERR_INVALID_ARGUMENT;
    }

    for (size_t i = 0; i < sv.len; ++i) {
        if (sv.data[i] == ch) {
            *out_index = i;
            return YW_SUCCESS;
        }
    }

    return YW_ERR_NOT_FOUND;
}

yw_result yw_memcpy_safe(void* dest, size_t dest_capacity, const void* src, size_t count) {
    if (dest == nullptr || src == nullptr) {
        return YW_ERR_INVALID_ARGUMENT;
    }
    if (count > dest_capacity) {
        return YW_ERR_OUT_OF_BOUNDS;
    }

    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    if ((d >= s && d < s + count) || (s >= d && s < d + count)) {
        return YW_ERR_INVALID_ARGUMENT;
    }

    for (size_t i = 0; i < count; ++i) {
        d[i] = s[i];
    }

    return YW_SUCCESS;
}