#include "libyw/math.h"

#define YW_PI          3.14159265358979323846
#define YW_TWO_PI      6.28318530717958647692
#define YW_HALF_PI     1.57079632679489661923

#define YW_PI_F        3.14159265358979323846f
#define YW_TWO_PI_F    6.28318530717958647692f
#define YW_HALF_PI_F   1.57079632679489661923f

static float normalize_radians_f(float x) {
    while (x > YW_PI_F)  x -= YW_TWO_PI_F;
    while (x < -YW_PI_F) x += YW_TWO_PI_F;
    return x;
}

float yw_math_sinf(float x) {
    x = normalize_radians_f(x);

    float x2 = x * x;
    float x3 = x2 * x;
    float x5 = x3 * x2;
    float x7 = x5 * x2;
    float x9 = x7 * x2;

    return x 
         - (x3 / 6.0f) 
         + (x5 / 120.0f) 
         - (x7 / 5040.0f) 
         + (x9 / 362880.0f);
}

float yw_math_cosf(float x) {
    return yw_math_sinf(x + YW_HALF_PI_F);
}

float yw_math_sqrtf(float x) {
    if (x < 0.0f) return 0.0f;
    if (x == 0.0f) return 0.0f;

    uint32_t i = yw_float_to_bits(x);
    i = 0x1fbd1df5 + (i >> 1);
    float y = yw_bits_to_float(i);

    y = 0.5f * (y + x / y);
    y = 0.5f * (y + x / y);
    y = 0.5f * (y + x / y);

    return y;
}

yw_sincosf_result yw_math_sincosf(float x) {
    return (yw_sincosf_result){
        .sin = yw_math_sinf(x),
        .cos = yw_math_cosf(x)
    };
}

yw_frexpf_result yw_math_frexpf(float x) {
    if (x == 0.0f) {
        return (yw_frexpf_result){ .fraction = 0.0f, .exponent = 0 };
    }

    uint32_t bits = yw_float_to_bits(x);
    int32_t exp = (int32_t)((bits >> 23) & 0xFF) - 127;

    if (exp == -127) {
        return (yw_frexpf_result){ .fraction = 0.0f, .exponent = 0 };
    }

    bits = (bits & 0x807FFFFFu) | (126u << 23);
    
    return (yw_frexpf_result){
        .fraction = yw_bits_to_float(bits),
        .exponent = exp + 1
    };
}

static double normalize_radians(double x) {
    while (x > YW_PI)  x -= YW_TWO_PI;
    while (x < -YW_PI) x += YW_TWO_PI;
    return x;
}

double yw_math_sin(double x) {
    x = normalize_radians(x);

    double x2 = x * x;
    double x3 = x2 * x;
    double x5 = x3 * x2;
    double x7 = x5 * x2;
    double x9 = x7 * x2;
    double x11 = x9 * x2;
    double x13 = x11 * x2;

    return x 
         - (x3 / 6.0) 
         + (x5 / 120.0) 
         - (x7 / 5040.0) 
         + (x9 / 362880.0) 
         - (x11 / 39916800.0) 
         + (x13 / 6227020800.0);
}

double yw_math_cos(double x) {
    return yw_math_sin(x + YW_HALF_PI);
}

double yw_math_sqrt(double x) {
    if (x < 0.0) return 0.0;
    if (x == 0.0) return 0.0;

    union { double d; uint64_t u; } cast = { .d = x };
    cast.u = (1ULL << 61) + (cast.u >> 1) - (1ULL << 51);
    double y = cast.d;

    y = 0.5 * (y + x / y);
    y = 0.5 * (y + x / y);
    y = 0.5 * (y + x / y);
    y = 0.5 * (y + x / y);

    return y;
}

yw_sincos_result yw_math_sincos(double x) {
    return (yw_sincos_result){
        .sin = yw_math_sin(x),
        .cos = yw_math_cos(x)
    };
}

yw_frexp_result yw_math_frexp(double x) {
    if (x == 0.0) {
        return (yw_frexp_result){ .fraction = 0.0, .exponent = 0 };
    }

    union { double d; uint64_t u; } cast = { .d = x };
    int32_t exp = (int32_t)((cast.u >> 52) & 0x7FF) - 1023;

    cast.u = (cast.u & 0x800FFFFFFFFFFFFFULL) | (1022ULL << 52);

    return (yw_frexp_result){
        .fraction = cast.d,
        .exponent = exp + 1
    };
}

yw_f32x4 yw_vec_sinf(yw_f32x4 v) {
    return (yw_f32x4){
        yw_math_sinf(v[0]),
        yw_math_sinf(v[1]),
        yw_math_sinf(v[2]),
        yw_math_sinf(v[3])
    };
}

yw_f32x4 yw_vec_sqrtf(yw_f32x4 v) {
    return (yw_f32x4){
        yw_math_sqrtf(v[0]),
        yw_math_sqrtf(v[1]),
        yw_math_sqrtf(v[2]),
        yw_math_sqrtf(v[3])
    };
}
