#pragma once
#include <stdio.h>

#define ARENA_SIZE 1024 * 1024 * 4 // 4 MB allocated initialy
#define PADDING 7

typedef struct {
    char *buffer;
    size_t offset;
    size_t capacity;
} Arena;

void init_arena(size_t size);
void *arena_allocate(size_t bytes);
void arena_free();
void arena_curr();
