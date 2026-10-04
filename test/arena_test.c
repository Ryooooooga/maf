#include "maf/arena.h"

#include "test.h" // IWYU pragma: keep

#include <stdint.h>
#include <string.h>

TEST(maf_arena_new) {
    SUBTEST("new and delete") {
        maf_arena *const arena = maf_arena_new();
        ASSERT(arena, ne(nullptr));

        maf_arena_delete(arena);
    }
}

TEST(maf_arena_new_with_block_size) {
    SUBTEST("minimum block size") {
        maf_arena *const arena =
            maf_arena_new_with_block_size(maf_arena_min_block_size);
        ASSERT(arena, ne(nullptr));

        maf_arena_delete(arena);
    }
    SUBTEST("maximum block size") {
        maf_arena *const arena =
            maf_arena_new_with_block_size(maf_arena_max_block_size);
        ASSERT(arena, ne(nullptr));

        maf_arena_delete(arena);
    }
}

TEST(maf_arena_delete) {
    SUBTEST("new and delete") {
        maf_arena *const arena = maf_arena_new();
        ASSERT(arena, ne(nullptr));

        maf_arena_delete(arena);
    }

    SUBTEST("delete null") {
        maf_arena_delete(nullptr);
    }
}

TEST(maf_arena_alloc) {
    SUBTEST("simple") {
        maf_arena *const arena = maf_arena_new();

        int *const p = maf_arena_alloc(arena, sizeof(int));
        ASSERT(p, ne(nullptr));

        *p = 42;
        EXPECT(*p, eq(42));

        maf_arena_delete(arena);
    }

    SUBTEST("zero") {
        maf_arena *const arena = maf_arena_new();

        void *const p = maf_arena_alloc(arena, 0);
        EXPECT(p, eq(nullptr));

        maf_arena_delete(arena);
    }

    SUBTEST("alignment") {
        maf_arena *const arena = maf_arena_new();

        for (size_t size = 1; size <= 64; size++) {
            void *const p = maf_arena_alloc(arena, size);
            ASSERT(p, ne(nullptr));
            EXPECT((uintptr_t)p % maf_arena_alignment, eq(0u));
        }

        maf_arena_delete(arena);
    }

    SUBTEST("many allocations") {
        maf_arena *const arena = maf_arena_new();

        // Allocates enough to span multiple blocks.
        constexpr size_t count = 10000;
        constexpr size_t size = 100;
        unsigned char *ptrs[count];

        for (size_t i = 0; i < count; i++) {
            ptrs[i] = maf_arena_alloc(arena, size);
            ASSERT(ptrs[i], ne(nullptr));
            memset(ptrs[i], (int)(i & 0xff), size);
        }

        for (size_t i = 0; i < count; i++) {
            for (size_t j = 0; j < size; j++) {
                ASSERT(ptrs[i][j], eq((unsigned char)(i & 0xff)));
            }
        }

        maf_arena_delete(arena);
    }

    SUBTEST("larger than block size") {
        maf_arena *const arena = maf_arena_new();

        unsigned char *const small1 = maf_arena_alloc(arena, 16);
        ASSERT(small1, ne(nullptr));
        memset(small1, 0x11, 16);

        constexpr size_t large_size = maf_arena_default_block_size * 4;
        unsigned char *const large = maf_arena_alloc(arena, large_size);
        ASSERT(large, ne(nullptr));
        memset(large, 0x22, large_size);

        // The small allocation following the large one should still be served
        // from the first block.
        unsigned char *const small2 = maf_arena_alloc(arena, 16);
        ASSERT(small2, ne(nullptr));
        EXPECT(small2, eq(small1 + 16));
        memset(small2, 0x33, 16);

        for (size_t i = 0; i < 16; i++) {
            ASSERT(small1[i], eq(0x11));
            ASSERT(small2[i], eq(0x33));
        }
        for (size_t i = 0; i < large_size; i++) {
            ASSERT(large[i], eq(0x22));
        }

        maf_arena_delete(arena);
    }
}

TEST(MAF_ARENA_ALLOC) {
    typedef struct pair_t {
        int a;
        int b;
    } pair_t;

    maf_arena *const arena = maf_arena_new();

    pair_t *const p = MAF_ARENA_ALLOC(arena, pair_t);
    ASSERT(p, ne(nullptr));

    *p = (pair_t){
        .a = 1,
        .b = 2,
    };
    EXPECT(p->a, eq(1));
    EXPECT(p->b, eq(2));

    maf_arena_delete(arena);
}

TEST(MAF_ARENA_ALLOC_ARRAY) {
    maf_arena *const arena = maf_arena_new();

    int *const p = MAF_ARENA_ALLOC_ARRAY(arena, int, 8);
    ASSERT(p, ne(nullptr));

    for (int i = 0; i < 8; i++) {
        p[i] = i;
    }
    for (int i = 0; i < 8; i++) {
        EXPECT(p[i], eq(i));
    }

    maf_arena_delete(arena);
}
