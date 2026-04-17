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
extern bool initialized;

void test_pass(void);
void test_fail(void);
void test_allocator_align_frame(void);
void test_allocator_align_size(void);
void test_allocator_memory_copy(void);
void test_allocator_memory_move(void);
void test_exception_error_handler(void);
void test_exception_get_symbol(void);
void test_exception_manual(void);
void test_exception_plic(void);
void test_paging_align_address(void);
void test_paging_pa_to_ppn(void);
void test_paging_ppn_to_pa(void);
void test_serial_cursor(void);
void test_serial_format(void);
void test_serial_print(void);
void test_serial_print_num_bases(void);
void test_string_char_is_empty(void);
void test_string_clear(void);
void test_string_compare(void);
void test_string_length(void);
void test_string_memory_set(void);
void test_string_to_num(void);
void test_string_to_num_ext(void);
void test_terminal_parse_arguments(void);
void test_terminal_parse_command(void);
void test_timer_parse_time(void);
void test_timer_set_time(void);
