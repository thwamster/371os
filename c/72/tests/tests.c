#include "tests.h"
#include "../src/library.h"
#include "../src/serial.h"
#include "../src/system.h"

extern struct Test tests[] = {{test_system_error_handler, false}, {test_serial_print, true}, {test_library_str_cmp, true}, {test_system_exception, false}};

size_t test_index = 0;
size_t test_count = sizeof(tests) / sizeof(tests[0]);
uint32_t fail_count = 0;

void assert(bool expression) {
	if (!expression) { throw_exception(); }
}

void test_system_error_handler(void) { assert(false); }

void test_serial_print(void) {
	print("0");
	print_reset("0");
	print_line("0");
	print_char('0');
	print_num('0', 10);
	print_line("");
}

void test_library_str_cmp(void) {
	assert(strcmp("0", "0") == 0);
	assert(strcmp("0", "1") != 0);
}

void test_system_exception(void) { throw_exception(); }