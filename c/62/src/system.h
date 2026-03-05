#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static volatile uint32_t * SIFIVE = (uint32_t *) 0x100000;
static volatile uint32_t SIFIVE_PASS = 0x5555;
static volatile uint32_t SIFIVE_FAIL = 0x3333;

void exit(uint32_t code);
void handler(void);