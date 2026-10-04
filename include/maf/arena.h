#pragma once

#include <stddef.h>
#include <stdint.h>

constexpr size_t maf_arena_default_block_size = 4096;
constexpr size_t maf_arena_max_block_size = 1 << 30; // 1 GiB
constexpr size_t maf_arena_min_block_size = 256;
constexpr size_t maf_arena_max_alloc_size = SIZE_MAX >> 1;
constexpr size_t maf_arena_alignment = alignof(max_align_t);

typedef struct maf_arena maf_arena;

[[nodiscard]]
maf_arena *maf_arena_new();

[[nodiscard]]
maf_arena *maf_arena_new_with_block_size(size_t block_size);

void maf_arena_delete(maf_arena *arena);

[[nodiscard]]
void *maf_arena_alloc(maf_arena *arena, size_t size);

#define MAF_ARENA_ALLOC(arena, type)                                           \
    ((type *)maf_arena_alloc((arena), sizeof(type)))

#define MAF_ARENA_ALLOC_ARRAY(arena, type, count)                              \
    ((type *)maf_arena_alloc((arena), sizeof(type) * (count)))
