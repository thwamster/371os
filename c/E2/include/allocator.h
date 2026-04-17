#pragma once

#include <stddef.h>
#include <stdint.h>

struct Block {
	struct Block * next;
	size_t size;
};

static const uint64_t FRAME_SIZE = 4096;
static const uint64_t BLOCK_SIZE = sizeof(struct Block);
static const uint64_t POINTER_SIZE = 7;
static const uint64_t MEMORY_START = 0x80000000;
static const uint64_t MEMORY_SIZE = 0x8000000;
static const uint64_t MEMORY_END = MEMORY_START + MEMORY_SIZE;

extern char end;
extern volatile uint64_t next_frame;

void frame_initialize(void);
uint64_t frame_allocate(void);
uint64_t frame_allocate_ext(size_t count);
void * memory_allocate(size_t size);
void memory_deallocate(void * pointer);
void * memory_set(void * destination, uint8_t value, size_t size);
void * memory_copy(void * restrict destination, const void * source, size_t size);
void * memory_move(void * restrict destination, const void * source, size_t size);
uint64_t align_frame(uint64_t address);
size_t align_size(size_t size);