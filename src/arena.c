#include "maf/arena.h"

#include "maf/malloc.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static size_t maf_align_up(size_t value) {
    assert(value <= SIZE_MAX - (maf_arena_alignment - 1) && "size overflow");

    return (value + maf_arena_alignment - 1) & ~(maf_arena_alignment - 1);
}

typedef struct maf_arena_block {
    struct maf_arena_block *next;
    size_t capacity;
    size_t used;
    alignas(maf_arena_alignment) char data[];
} maf_arena_block;

[[nodiscard]]
static maf_arena_block *maf_arena_block_new(maf_arena_block *next,
                                            size_t capacity) {
    assert(capacity <= maf_arena_max_alloc_size);

    auto const block =
        (maf_arena_block *)maf_malloc(sizeof(maf_arena_block) + capacity);
    *block = (maf_arena_block){
        .next = next,
        .capacity = capacity,
        .used = 0,
    };

    return block;
}

static void maf_arena_block_delete(maf_arena_block *block) {
    maf_free(block);
}

[[nodiscard]]
static void *maf_arena_block_try_alloc(maf_arena_block *block,
                                       size_t aligned_size) {
    assert(block != nullptr);
    assert(aligned_size == maf_align_up(aligned_size));

    if (block->capacity - block->used < aligned_size) {
        return nullptr;
    }

    void *const p = block->data + block->used;
    block->used += aligned_size;

    return p;
}

struct maf_arena {
    maf_arena_block *head;
    size_t next_block_size;
};

static size_t maf_arena_next_block_size(size_t current_block_size) {
    auto const next_block_size = current_block_size * 2;
    return next_block_size <= maf_arena_max_block_size
               ? next_block_size
               : maf_arena_max_block_size;
}

[[nodiscard]]
maf_arena *maf_arena_new() {
    return maf_arena_new_with_block_size(maf_arena_default_block_size);
}

[[nodiscard]]
maf_arena *maf_arena_new_with_block_size(size_t block_size) {
    assert(block_size >= maf_arena_min_block_size);
    assert(block_size <= maf_arena_max_block_size);

    auto const arena = MAF_MALLOC(maf_arena);
    *arena = (maf_arena){
        .head = maf_arena_block_new(nullptr, block_size),
        .next_block_size = maf_arena_next_block_size(block_size),
    };

    return arena;
}

void maf_arena_delete(maf_arena *arena) {
    if (arena == nullptr) {
        return;
    }

    auto block = arena->head;
    while (block != nullptr) {
        auto const next = block->next;
        maf_arena_block_delete(block);
        block = next;
    }

    maf_free(arena);
}

[[nodiscard]]
void *maf_arena_alloc(maf_arena *arena, size_t size) {
    assert(arena != nullptr);
    assert(size <= maf_arena_max_alloc_size);

    if (size == 0) {
        return nullptr;
    }

    auto const aligned_size = maf_align_up(size);

    void *const p = maf_arena_block_try_alloc(arena->head, aligned_size);
    if (p != nullptr) {
        return p;
    }

    maf_arena_block *block;
    if (aligned_size > arena->next_block_size) {
        // Give oversized allocations a dedicated block, linked behind the
        // current head so that the head's remaining space is not wasted.
        block = maf_arena_block_new(arena->head->next, aligned_size);
        arena->head->next = block;
    } else {
        block = maf_arena_block_new(arena->head, arena->next_block_size);
        arena->head = block;

        arena->next_block_size =
            maf_arena_next_block_size(arena->next_block_size);
    }

    return maf_arena_block_try_alloc(block, aligned_size);
}
