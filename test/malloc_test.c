#include "maf/malloc.h"

#include "test.h" // IWYU pragma: keep

TEST(maf_malloc) {
    SUBTEST("simple") {
        int *const p = maf_malloc(sizeof(int));
        ASSERT(p, ne(nullptr));

        *p = 42;
        EXPECT(*p, eq(42));

        maf_free(p);
    }

    SUBTEST("zero") {
        void *const p = maf_malloc(0);
        EXPECT(p, eq(nullptr));
    }
}

TEST(maf_realloc) {
    SUBTEST("simple") {
        int *p = maf_malloc(sizeof(int) * 4);
        ASSERT(p, ne(nullptr));

        for (int i = 0; i < 4; i++) {
            p[i] = i;
        }

        p = maf_realloc(p, sizeof(int) * 1024);
        ASSERT(p, ne(nullptr));

        for (int i = 0; i < 4; i++) {
            EXPECT(p[i], eq(i));
        }

        maf_free(p);
    }

    SUBTEST("zero") {
        int *p = maf_malloc(sizeof(int) * 4);
        ASSERT(p, ne(nullptr));

        for (int i = 0; i < 4; i++) {
            p[i] = i;
        }

        p = maf_realloc(p, 0);
        EXPECT(p, eq(nullptr));
    }

    SUBTEST("null") {
        int *const p = maf_realloc(nullptr, sizeof(int) * 4);
        ASSERT(p, ne(nullptr));

        for (int i = 0; i < 4; i++) {
            p[i] = i;
        }

        for (int i = 0; i < 4; i++) {
            EXPECT(p[i], eq(i));
        }

        maf_free(p);
    }
}

TEST(MAF_MALLOC) {
    typedef struct pair_t {
        int a;
        int b;
    } pair_t;

    pair_t *const p = MAF_MALLOC(pair_t);
    ASSERT(p, ne(nullptr));

    *p = (pair_t){
        .a = 1,
        .b = 2,
    };
    EXPECT(p->a, eq(1));
    EXPECT(p->b, eq(2));

    maf_free(p);
}

TEST(MAF_MALLOC_ARRAY) {
    int *const p = MAF_MALLOC_ARRAY(int, 8);
    ASSERT(p, ne(nullptr));

    for (int i = 0; i < 8; i++) {
        p[i] = i;
    }
    for (int i = 0; i < 8; i++) {
        EXPECT(p[i], eq(i));
    }

    maf_free(p);
}

TEST(MAF_REALLOC_ARRAY) {
    int *p = MAF_MALLOC_ARRAY(int, 4);
    ASSERT(p, ne(nullptr));

    for (int i = 0; i < 4; i++) {
        p[i] = i;
    }

    p = MAF_REALLOC_ARRAY(p, 1024);
    ASSERT(p, ne(nullptr));

    for (int i = 0; i < 4; i++) {
        EXPECT(p[i], eq(i));
    }

    p[1023] = 42;
    EXPECT(p[1023], eq(42), "Ensure the whole requested range is writable.");

    maf_free(p);
}
