#include "libyw/string.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    printf("[libyw] Running string tests...\n");

    yw_string_view sv1 = yw_sv_from_parts("Hello, C23!", 11);
    yw_string_view sv2 = yw_sv_from_parts("Hello, C23!", 11);
    yw_string_view sv3 = yw_sv_from_parts("Hello, World", 12);

    assert(yw_sv_equals(sv1, sv2) == true);
    assert(yw_sv_equals(sv1, sv3) == false);
    (void)sv1; (void)sv2; (void)sv3;

    size_t found_idx = 0;
    yw_result res = yw_sv_find_char(sv1, 'C', &found_idx);
    assert(res == YW_SUCCESS);
    assert(found_idx == 7);
    (void)res; (void)found_idx;

    char buffer[5];
    char src[] = "TooLong";
    yw_result copy_res = yw_memcpy_safe(buffer, sizeof(buffer), src, 7);
    assert(copy_res == YW_ERR_OUT_OF_BOUNDS);
    (void)buffer; (void)src; (void)copy_res;

    printf("[libyw] All string tests passed successfully!\n");
    return 0;
}
