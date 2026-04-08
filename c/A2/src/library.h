#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void * memory_set(void * dest, register uint8_t val, register size_t len);
size_t str_len(const char * s);
int8_t str_comp(const char * string1, const char * string2);
int64_t str_to_num(const char * string, uint8_t base);
int64_t str_to_num_len(const char * string, uint8_t base, size_t length);
void str_clear(char * buffer, size_t size);
bool char_is_empty(char c);