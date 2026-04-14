#pragma once

#include <stdint.h>

static const uint64_t FRAME_SIZE = 4096;
static const uint64_t MEMORY_START = 0x80000000;
static const uint64_t MEMORY_SIZE = 0x8000000;
static const uint64_t MEMORY_END = MEMORY_START + MEMORY_SIZE;

extern char end;
extern volatile uint64_t next_frame;

void initialize_frame(void);
uint64_t allocate_frame(void);