#include "libyw/math.h"

#define YW_PI          3.14159265358979323846
#define YW_TWO_PI      6.28318530717958647692
#define YW_HALF_PI     1.57079632679489661923
#define YW_INV_TWO_PI  0.159154943091895335768

#define YW_PI_F         3.14159265358979323846f
#define YW_TWO_PI_F     6.28318530717958647692f
#define YW_HALF_PI_F    1.57079632679489661923f
#define YW_INV_TWO_PI_F 0.159154943091895335768f

static const float S_COEFF_F[] = {
    1.0f,
   -0.166666666416265235595f,
    0.0083333318073862412854f,
   -0.0001984090023707833621f,
    0.000002752600208985165f
};

static float normalize_radians_f(float x) {
    float k = (float)(int)(x * YW_INV_TWO_PI_F + (x >= 0.0f ? 0.5f : -0.5f));
    return x - k * YW_TWO_PI_F;
}

float yw_math_sinf(float x) {
    x = normalize_radians_f(x);

    if (x > YW_HALF_PI_F) {
        x = YW_PI_F - x;
    } else if (x < -YW_HALF_PI_F) {
        x = -YW_PI_F - x;
    }

    float x2 = x * x;
    float poly = S_COEFF_F[0] + x2 * (S_COEFF_F[1] + x2 * (S_COEFF_F[2] + x2 * (S_COEFF_F[3] + x2 * S_COEFF_F[4])));

    return x * poly;
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
    float s = yw_math_sinf(x);
    float c = yw_math_cosf(x);
    return (yw_sincosf_result){ .sin = s, .cos = c };
}

yw_frexpf_result yw_math_frexpf(float x) {
    if (x == 0.0f) {
        return (yw_frexpf_result){ .fraction = 0.0f, .exponent = 0 };
    }

    uint32_t bits = yw_float_to_bits(x);
    int32_t exp = (int32_t)((bits >> 23) & 0xFF) - 127;

    if ((bits & 0x7FFFFFFF) == 0) {
        return (yw_frexpf_result){ .fraction = x, .exponent = 0 };
    }

    bits = (bits & 0x807FFFFFu) | (126u << 23);
    
    return (yw_frexpf_result){
        .fraction = yw_bits_to_float(bits),
        .exponent = exp + 1
    };
}

static double normalize_radians(double x) {
    double k = (double)(int64_t)(x * YW_INV_TWO_PI + (x >= 0.0 ? 0.5 : -0.5));
    return x - k * YW_TWO_PI;
}

double yw_math_sin(double x) {
    x = normalize_radians(x);

    if (x > YW_HALF_PI) {
        x = YW_PI - x;
    } else if (x < -YW_HALF_PI) {
        x = -YW_PI - x;
    }

    double x2 = x * x;
    return x * (1.0 + x2 * (-1.0/6.0 + x2 * (1.0/120.0 + x2 * (-1.0/5040.0 + x2 * (1.0/362880.0 + x2 * (-1.0/39916800.0 + x2 * (1.0/6227020800.0)))))));
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
    yw_f32x4 x = v;
    for (int i = 0; i < 4; ++i) {
        x[i] = normalize_radians_f(v[i]);
        if (x[i] > YW_HALF_PI_F)       x[i] = YW_PI_F - x[i];
        else if (x[i] < -YW_HALF_PI_F) x[i] = -YW_PI_F - x[i];
    }

    yw_f32x4 x2 = x * x;
    yw_f32x4 poly = S_COEFF_F[0] 
                  + x2 * (S_COEFF_F[1] 
                  + x2 * (S_COEFF_F[2] 
                  + x2 * (S_COEFF_F[3] 
                  + x2 * S_COEFF_F[4])));

    return x * poly;
}

yw_f32x4 yw_vec_cosf(yw_f32x4 v) {
    return yw_vec_sinf(v + (yw_f32x4){ YW_HALF_PI_F, YW_HALF_PI_F, YW_HALF_PI_F, YW_HALF_PI_F });
}

yw_f32x4 yw_vec_sqrtf(yw_f32x4 v) {
    yw_f32x4 res;
    for (int i = 0; i < 4; ++i) {
        res[i] = yw_math_sqrtf(v[i]);
    }
    return res;
}
