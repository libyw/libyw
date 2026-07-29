#ifndef LIBYW_CORE_TYPES_H
#define LIBYW_CORE_TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define YW_INLINE static inline

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    #define YW_IS_C23 1
#else
    #define YW_IS_C23 0
#endif

#if YW_IS_C23
    #define YW_NODISCARD [[nodiscard]]
    #define YW_NORETURN  [[noreturn]]
#else
    #ifndef nullptr
        #define nullptr NULL
    #endif

    #if defined(__GNUC__) || defined(__clang__)
        #define YW_NODISCARD __attribute__((warn_unused_result))
        #define YW_NORETURN  __attribute__((noreturn))
    #elif defined(_MSC_VER)
        #define YW_NODISCARD _Check_return_
        #define YW_NORETURN  __declspec(noreturn)
    #else
        #define YW_NODISCARD
        #define YW_NORETURN
    #endif
#endif

typedef uint32_t yw_mode_t;

typedef enum 
#if YW_IS_C23
 : int32_t 
#endif 
{
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

#if defined(__INTELLISENSE__)
#define YW_REGISTER
#else
#define YW_REGISTER register
#endif

#endif // LIBYW_CORE_TYPES_H
