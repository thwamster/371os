#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct Test {
	void (*test)(void);
	bool expecting_pass;
};

extern struct Test tests[];
extern size_t test_index;
extern size_t test_count;
extern uint32_t fail_count;

void assert(bool expression);
void test_system_exception(void);
void test_system_error_handler(void);
void test_system_get_symbol(void);
void test_system_plic(void);
void test_serial_print(void);
void test_serial_print_num_bases(void);
void test_serial_format(void);
void test_serial_cursor(void);
void test_library_str_cmp(void);
void test_library_str_len(void);
void test_library_str_to_num(void);
void test_library_str_clear(void);
void test_library_char_is_empty(void);
void test_library_memory_set(void);
void test_timer_set_time(void);
void test_timer_parse_time(void);