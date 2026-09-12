#include "arena.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

static Arena arena;

void init_arena(size_t size) {
    arena.buffer = malloc(size);
    arena.capacity = size;
    arena.offset = 0;
}

void *arena_allocate(size_t bytes) {
    // bytes get padded and then first 3 bits are set to 0, so that the memory is aligned in the multiples of 8 (when PADDING is 7)
    size_t bytes_aligned = (bytes + PADDING) & ~PADDING;

    if (arena.offset + bytes_aligned > arena.capacity) {
        errx(1, "Error: not enough memory.");
    }

    void *ptr = &arena.buffer[arena.offset];
    arena.offset += bytes_aligned;
    return ptr;
}

void arena_free() {
    free(arena.buffer);
}

void arena_curr() {
    printf("%d\n", (int)arena.offset);
}
