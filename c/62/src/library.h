#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static volatile uint32_t *SIFIVE = (uint32_t *) 0x100000;

void exit(int code);
void *memset(void *dest, register int val, register size_t len);
int strcmp(const char *string1, const char *string2);