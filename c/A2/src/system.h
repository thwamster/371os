#pragma once

#include <stdint.h>

#define MANUAL_EXCEPTION *(int *) 0 = 0
#define CAUSE_INTERRUPT (1ULL << 63)
#define CAUSE_CODE (~CAUSE_INTERRUPT)

static volatile uint32_t * const SIFIVE = (uint32_t *) 0x100000;
static const uint32_t SIFIVE_PASS = 0x5555;
static const uint32_t SIFIVE_FAIL = 0x3333;

void exit(uint32_t code);
__attribute__((naked, aligned(4))) void handler(void);
void throw_exception(void);
uint64_t get_exception(void);