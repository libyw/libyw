#ifndef LIBYW_MATH_H
#define LIBYW_MATH_H

#include "libyw/core/types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define YW_MATH_PI          3.14159265358979323846
#define YW_MATH_PI_F        3.14159265358979323846f
#define YW_MATH_E           2.71828182845904523536
#define YW_MATH_SQRT2       1.41421356237309504880

typedef float yw_f32x4 [[gnu::vector_size(16)]];

typedef double yw_f64x2 [[gnu::vector_size(16)]];

typedef struct {
    float sin;
    float cos;
} yw_sincosf_result;

typedef struct {
    double sin;
    double cos;
} yw_sincos_result;

typedef struct {
    float fraction;
    int exponent;
} yw_frexpf_result;

typedef struct {
    double fraction;
    int exponent;
} yw_frexp_result;

YW_NODISCARD float yw_math_sinf(float x);
YW_NODISCARD float yw_math_cosf(float x);
YW_NODISCARD float yw_math_sqrtf(float x);
YW_NODISCARD yw_sincosf_result yw_math_sincosf(float x);
YW_NODISCARD yw_frexpf_result yw_math_frexpf(float x);

YW_NODISCARD double yw_math_sin(double x);
YW_NODISCARD double yw_math_cos(double x);
YW_NODISCARD double yw_math_sqrt(double x);
YW_NODISCARD yw_sincos_result yw_math_sincos(double x);
YW_NODISCARD yw_frexp_result yw_math_frexp(double x);

YW_NODISCARD yw_f32x4 yw_vec_sinf(yw_f32x4 v);
YW_NODISCARD yw_f32x4 yw_vec_sqrtf(yw_f32x4 v);

#define yw_sin(x) _Generic((x), \
    float:  yw_math_sinf,       \
    double: yw_math_sin         \
)(x)

#define yw_cos(x) _Generic((x), \
    float:  yw_math_cosf,       \
    double: yw_math_cos         \
)(x)

#define yw_sqrt(x) _Generic((x), \
    float:  yw_math_sqrtf,       \
    double: yw_math_sqrt         \
)(x)

#define yw_sincos(x) _Generic((x), \
    float:  yw_math_sincosf,       \
    double: yw_math_sincos         \
)(x)

#define yw_frexp(x) _Generic((x),  \
    float:  yw_math_frexpf,        \
    double: yw_math_frexp          \
)(x)

YW_NODISCARD YW_INLINE uint32_t yw_float_to_bits(float f) {
#if __has_builtin(__builtin_bit_cast)
    return __builtin_bit_cast(uint32_t, f);
#else
    union { float f; uint32_t u; } cast = { .f = f };
    return cast.u;
#endif
}

YW_NODISCARD YW_INLINE float yw_bits_to_float(uint32_t u) {
#if __has_builtin(__builtin_bit_cast)
    return __builtin_bit_cast(float, u);
#else
    union { uint32_t u; float f; } cast = { .u = u };
    return cast.f;
#endif
}

#ifdef __cplusplus
}
#endif

#endif // LIBYW_MATH_H
