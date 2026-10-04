#pragma once

#include <stddef.h>

// Returns nullptr if `size` is 0.
// Aborts on allocation failure.
[[nodiscard]]
void *maf_malloc(size_t size);

// Frees `ptr` and returns nullptr if `size` is 0.
// Aborts on allocation failure.
[[nodiscard]]
void *maf_realloc(void *ptr, size_t size);

void maf_free(void *ptr);

#define MAF_MALLOC(type) ((type *)maf_malloc(sizeof(type)))

#define MAF_MALLOC_ARRAY(type, count)                                          \
    ((type *)maf_malloc(sizeof(type) * (count)))

#define MAF_REALLOC_ARRAY(ptr, count)                                          \
    ((typeof_unqual(*(ptr)) *)maf_realloc(                                     \
        (ptr), sizeof(typeof_unqual(*(ptr))) * (count)))
