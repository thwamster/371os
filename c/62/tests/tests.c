#include "tests.h"
#include "../src/library.h"
#include "../src/serial.h"

extern struct Test tests[] = {
	{test_system_error_handler, false},
	{test_serial_print_line, true},
	{test_library_strcmp, true},
};

size_t test_index = 0;
size_t test_count = sizeof(tests) / sizeof(tests[0]);
uint32_t fail_count = 0;

void assert(bool expression) {
	if (!expression) { *(int *) 0 = 0; }
}

void test_system_error_handler(void) { assert(false); }

void test_serial_print_line(void) {
	print("0");
	print_reset("0");
	print_line("0");
	print_char('0');
	print_int('0', 10);
	print_line("");
}

void test_library_strcmp(void) {
	assert(strcmp("0", "0") == 0);
	assert(strcmp("0", "1") != 0);
}