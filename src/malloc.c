#include "maf/malloc.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *maf_malloc(size_t size) {
    if (size == 0) {
        return nullptr;
    }

    void *const p = malloc(size);
    if (p == nullptr) {
        fprintf(stderr, "%s: failed to allocate %zu bytes: %s\n", __func__,
                size, strerror(errno));
        abort();
    }

    return p;
}

void *maf_realloc(void *ptr, size_t size) {
    if (size == 0) {
        free(ptr);
        return nullptr;
    }

    void *const p = realloc(ptr, size);
    if (p == nullptr) {
        fprintf(stderr, "%s: failed to allocate %zu bytes: %s\n", __func__,
                size, strerror(errno));
        abort();
    }

    return p;
}

void maf_free(void *ptr) {
    free(ptr);
}
