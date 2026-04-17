#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void * memset(void * dest, register int val, register size_t len);
int strcmp(const char * string1, const char * string2);
bool char_is_empty(char c);
int parse_int(const char * string);