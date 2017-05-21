#include "custom_realloc.h"
#include <stdlib.h>
#include <assert.h>

#define CMOK_DO_MOCK
#include "../cmok.h"

CMOK_DEFINE2(void*, custom_realloc, void*, ptr, size_t, size) {
    return realloc(ptr, size);
}

void* null_realloc(void* ptr, size_t size) { return NULL; }

void test_null_realloc_mock(void) {
    CMOK_FUN(custom_realloc) = null_realloc;
    assert(custom_realloc(0, 0) == NULL);
    CMOK_RESET(custom_realloc);
}
