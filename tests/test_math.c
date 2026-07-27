#include "libyw/math.h"
#include <assert.h>

int main(void) {
    float s_f = yw_sin(YW_MATH_PI_F / 2.0f);
    double s_d = yw_sin(YW_MATH_PI / 2.0);

    assert(s_f > 0.99f && s_f < 1.01f);
    assert(s_d > 0.9999 && s_d < 1.0001);

    uint32_t bits = yw_float_to_bits(1.0f);
    assert(bits == 0x3f800000);

    return 0;
}