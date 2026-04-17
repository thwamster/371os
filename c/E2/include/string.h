#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

size_t string_length(const char * s);
int8_t string_compare(const char * string1, const char * string2);
int64_t string_to_num(const char * string, uint8_t base);
int64_t string_to_num_ext(const char * string, uint8_t base, size_t length);
char * string_find_char(const char * string, char character);
void string_clear(char * buffer, size_t size);
bool char_is_empty(char c);