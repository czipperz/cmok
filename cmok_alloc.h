/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2017 Chris Gregory czipperz@gmail.com
 */

#ifndef HEADER_GUARD_CMOK_ALLOC_H
#define HEADER_GUARD_CMOK_ALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CMOK_MOCK_ALLOC
#define malloc cmok_malloc
#define calloc cmok_calloc
#define realloc cmok_realloc
#define free cmok_free
#endif

#include <stddef.h>

extern void* (*cmok_malloc_ptr)(size_t size);
extern void* (*cmok_calloc_ptr)(size_t num, size_t size);
extern void* (*cmok_realloc_ptr)(void* ptr, size_t newsize);
extern void (*cmok_free_ptr)(void* ptr);

void* cmok_malloc(const char* filename, size_t line,
                  size_t size);
void* cmok_calloc(const char* filename, size_t line,
                  size_t num, size_t size);
void* cmok_realloc(const char* filename, size_t line,
                   void* ptr, size_t newsize);
void cmok_free(const char* filename, size_t line,
               void* ptr);

struct cmok_alloc_error {
    const char* filename;
    size_t line;
    const char* description;
};
typedef struct cmok_alloc_error cmok_alloc_error;

struct cmok_alloc_errors {
    cmok_alloc_error* errors;
    size_t num;
};
typedef struct cmok_alloc_errors cmok_alloc_errors;

cmok_alloc_errors cmok_alloc_errors_get(void);
void cmok_alloc_errors_clear(void);

#ifdef __cplusplus
}
#endif

#endif
