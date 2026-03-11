#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void assert(bool expression);
void test_system_error_handler(void);
void test_serial_print_line(void);
void test_library_strcmp(void);
void test_manual_exception(void);

struct Test {
	void (*test)(void);
	bool expecting_pass;
};

extern struct Test tests[];
extern size_t test_index;
extern size_t test_count;
extern uint32_t fail_count;