#ifndef LIBYW_CONTAINER_RESULT_H
#define LIBYW_CONTAINER_RESULT_H

#include "libyw/core/types.h"
#include "libyw/sys/sys.h"

typedef enum {
    YW_RESULT_SUCCESS,
    YW_RESULT_FAILURE
} yw_result_status;

typedef struct yw_result {
    yw_result_status status;
    union {
        void* value;
        void* error;
    };
} yw_result;

YW_INLINE void yw_panic(const char* msg, const char* file, int line) {
    yw_sys_write(2, "Panic: ", 7);

    size_t len = 0;
    while (msg && msg[len]) len++;
    yw_sys_write(2, msg, len);

    yw_sys_write(2, " (at ", 5);
    
    size_t file_len = 0;
    while (file && file[file_len]) file_len++;
    yw_sys_write(2, file, file_len);

    yw_sys_write(2, ")\n", 2);

    yw_sys_exit(1);
}

#define RESULT_DEFINE(Name, T, E)                 \
    typedef struct {                              \
        yw_result_status status;                  \
        union {                                   \
            T value;                              \
            E error;                              \
        } as;                                     \
    } Name;                                       \
                                                  \
    YW_INLINE Name Name##_ok(T val) {      \
        Name r = { .status = YW_RESULT_SUCCESS }; \
        r.as.value = val;                         \
        return r;                                 \
    }                                             \
                                                  \
    YW_INLINE Name Name##_err(E err) {     \
        Name r = { .status = YW_RESULT_FAILURE }; \
        r.as.error = err;                         \
        return r;                                 \
    }

#define RESULT_UNWRAP(res, msg) __extension__ ({  \
    __typeof__(res) _res = (res);                 \
    if (_res.status != YW_RESULT_SUCCESS) {       \
        yw_panic((msg), __FILE__, __LINE__);      \
    }                                             \
    _res.as.value;                                \
})

RESULT_DEFINE(IntResult, int, const char*)
RESULT_DEFINE(SizeResult, size_t, const char*)
RESULT_DEFINE(DoubleResult, double, const char*)
RESULT_DEFINE(BoolResult, bool, const char*)
RESULT_DEFINE(PtrResult, void*, const char*)
RESULT_DEFINE(IntResultErr, int, int)
RESULT_DEFINE(SizeResultErr, size_t, int)
RESULT_DEFINE(PtrResultErr, void*, int)

#endif // LIBYW_CONTAINER_RESULT_H
