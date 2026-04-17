#pragma once

#include <stdbool.h>
#include <stdint.h>

extern bool random_seed;
extern uint64_t state;

void random_initialize();
uint64_t random_num();
uint32_t random_range(uint32_t min, uint32_t max);