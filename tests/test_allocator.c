#include "libyw/core/allocator.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    printf("[libyw] Testing allocator...\n");

    yw_allocator* alloc = yw_allocator_system();
    assert(alloc != nullptr);

    int* data = (int*)yw_alloc(alloc, sizeof(int) * 16, 16);
    assert(data != nullptr);

    for (int i = 0; i < 16; ++i) {
        data[i] = i * 10;
    }

    assert(data[5] == 50);

    yw_dealloc(alloc, data, sizeof(int) * 16);

    printf("[libyw] Allocator test passed!\n");
    return 0;
}