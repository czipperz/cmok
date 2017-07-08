#include "../cmok_alloc.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* (*cmok_malloc_ptr)(size_t size) = malloc;
void* (*cmok_calloc_ptr)(size_t num, size_t size) = calloc;
void* (*cmok_realloc_ptr)(void* ptr, size_t newsize) = realloc;
void (*cmok_free_ptr)(void* ptr) = free;
static cmok_alloc_errors errors = { 0, 0 };
static size_t cap = 0;
static void** ptrs = 0;

static int
alloc_another_error(void) {
    cmok_alloc_error* e = errors.errors;
    void** p = ptrs;
    size_t c = cap;
    if (errors.errors) {
        assert(errors.num <= cap);
        if (errors.num == cap) {
            c *= 2;
            e = realloc(e, c * sizeof(cmok_alloc_error));
            p = realloc(p, c * sizeof(void*));
            if (!e || !p) {
                if (e) { errors.errors = e; }
                if (p) { ptrs = p; }
                return 1;
            }
        } else {
            return 0;
        }
    } else {
        c = 16;
        e = malloc(c * sizeof(cmok_alloc_error));
        p = malloc(c * sizeof(void*));
        if (!e || !p) {
            free(e);
            free(p);
            return 1;
        }
    }
    errors.errors = e;
    ptrs = p;
    cap = c;
    return 0;
}

static void
add_alloc_error(const char* filename, size_t line,
                const char* description, void* ptr) {
    cmok_alloc_error error;
    error.filename = filename;
    error.line = line;
    error.description = description;

    errors.errors[errors.num] = error;
    ptrs[errors.num] = ptr;
    ++errors.num;
}

void*
cmok_malloc(const char* filename, size_t line,
            size_t size) {
    void* res;
    if (alloc_another_error()) { return 0; }
    res = cmok_malloc_ptr(size);
    add_alloc_error(filename, line,
                    "`malloc()` was called without an associated "
                    "`free()`.",
                    res);
    return res;
}

void*
cmok_calloc(const char* filename, size_t line,
            size_t num, size_t size) {
    void* res;
    if (alloc_another_error()) { return 0; }
    res = cmok_calloc_ptr(num, size);
    add_alloc_error(filename, line,
                    "`calloc()` was called without an associated "
                    "`free()`.", res);
    return res;
}

static int
check_error_free(const char* filename, size_t line,
                 void* ptr) {
    size_t iter;
    for (iter = 0; iter != errors.num; ++iter) {
        if (ptrs[iter] == ptr) {
            memmove(ptrs + iter, ptrs + iter + 1,
                    errors.num - (iter + 1));
            memmove(errors.errors + iter, errors.errors + iter + 1,
                    errors.num - (iter + 1));
            --errors.num;
            return 0;
        }
    }
    if (alloc_another_error()) {
        printf("%s:%lu: free() was called on an invalid pointer.  "
               "This message was spit out prematurely because of an "
               "allocation error.",
               filename, (unsigned long) line);
    } else {
        add_alloc_error(filename, line,
                        "free() was called on an invalid pointer.",
                        0);
    }
    return 1;
}

void*
cmok_realloc(const char* filename, size_t line,
             void* ptr, size_t newsize) {
    void* res;
    if (ptr) {
        if (check_error_free(filename, line, ptr)) { return 0; }
    }
    if (alloc_another_error()) { return 0; }
    res = cmok_realloc_ptr(ptr, newsize);
    add_alloc_error(filename, line,
                    "`realloc()` was called without an "
                    "associated `free()`.",
                    ptr);
    return res;
}

void
cmok_free(const char* filename, size_t line,
          void* ptr) {
    if (!ptr) {
        return;
    }
    check_error_free(filename, line, ptr);
    cmok_free_ptr(ptr);
}

cmok_alloc_errors cmok_alloc_errors_get(void) {
    return errors;
}

void cmok_alloc_errors_clear(void) {
    free(errors.errors);
}
