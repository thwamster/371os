#include "tests.h"
#include "library.h"
#include "serial.h"
#include "system.h"
#include "timer.h"

struct Test tests[] = {
	{test_exception_manual, false},		{test_exception_error_handler, false}, {test_exception_get_symbol, true}, {test_exception_plic, true},
	{test_serial_print, true},			{test_serial_print_num_bases, true},   {test_serial_format, true},		  {test_serial_cursor, true},
	{test_library_str_cmp, true},		{test_library_str_len, true},		   {test_library_str_to_num, true},	  {test_library_str_clear, true},
	{test_library_char_is_empty, true}, {test_library_memory_set, true},	   {test_timer_set_time, true},		  {test_timer_parse_time, true},
};

size_t test_index = 0;
size_t test_count = sizeof(tests) / sizeof(tests[0]);
uint32_t fail_count = 0;

void assert(const bool expression) {
	if (!expression) { throw_exception(); }
}

void test_exception_manual() { throw_exception(); }

void test_exception_error_handler() {
	assert(true);
	assert(false);
}

void test_exception_get_symbol() {
	if (symbol_count == 0) return;
	const struct Symbol * sym = get_symbol(symbol_table[0].address);
	assert(sym != (void *) 0);
	assert(sym->address == symbol_table[0].address);
	const struct Symbol * missing = get_symbol(0x0);
	assert(missing == (void *) 0);
}

void test_exception_plic() {
	const volatile uint32_t priority = *PLIC_PRIORITY;
	const volatile uint32_t enable = *PLIC_ENABLE;
	const volatile uint32_t threshold = *PLIC_THRESHOLD;
	(void) priority;
	(void) enable;
	(void) threshold;
}

void test_serial_print() {
	print("0");
	print_reset("0");
	print_char('0');
	print(" ");
}

void test_serial_print_num_bases() {
	print_num(255, 10);
	print(" ");
	print_num(255, 16);
	print(" ");
	print_num(255, 2);
	print(" ");
	print_num(-1, 10);
	print(" ");
	print_num(0, 16);
	print(" ");
	print_num_ext(255, 16, true, 4);
	print(" ");
	print_num_ext(7, 2, true, 8);
	print(" ");
	print_num_ext(-42, 10, false, 0);
	print(" ");
}

void test_serial_format() {
	format_style(BOLD);
	format_style(ITALIC);
	format_style(UNDERLINE);
	format_style(COLOR_FOREGROUND + RED);
	format_style(COLOR_FOREGROUND + GREEN);
	format_style(COLOR_BACKGROUND + BLUE);
	format_rgb(COLOR_FOREGROUND, 255, 128, 0);
	format_reset();
}

void test_serial_cursor() {
	cursor_visibility(false);
	cursor_move(RIGHT, 1);
	cursor_move(LEFT, 1);
	cursor_move(CURSOR_UP, 1);
	cursor_move(CURSOR_DOWN, 1);
	cursor_visibility(true);
}

void test_library_str_cmp() {
	assert(string_compare("0", "0") == 0);
	assert(string_compare("0", "1") != 0);
}

void test_library_str_len() {
	assert(string_length("") == 0);
	assert(string_length("hello") == 5);
	assert(string_length("0") == 1);
}

void test_library_str_to_num() {
	assert(string_to_num("0", 10) == 0);
	assert(string_to_num("1", 10) == 1);
	assert(string_to_num("255", 10) == 255);
	assert(string_to_num("ff", 16) == 255);
	assert(string_to_num("FF", 16) == 255);
	assert(string_to_num("11111111", 2) == 255);
	assert(string_to_num("-1", 10) == -1);
	assert(string_to_num_ext("1234", 10, 2) == 12);
}

void test_library_str_clear() {
	char buf[8];
	memory_set(buf, 0xFF, sizeof(buf));
	string_clear(buf, sizeof(buf));
	for (size_t i = 0; i < sizeof(buf); i++) { assert(buf[i] == '\0'); }
}

void test_library_char_is_empty() {
	assert(char_is_empty(' '));
	assert(char_is_empty('\t'));
	assert(char_is_empty('\n'));
	assert(char_is_empty('\r'));
	assert(!char_is_empty('a'));
	assert(!char_is_empty('0'));
}

void test_library_memory_set() {
	uint8_t buf[16];
	memory_set(buf, 0xAB, sizeof(buf));
	for (size_t i = 0; i < sizeof(buf); i++) { assert(buf[i] == 0xAB); }
	memory_set(buf, 0x00, sizeof(buf));
	for (size_t i = 0; i < sizeof(buf); i++) { assert(buf[i] == 0x00); }
}

void test_timer_set_time() {
	assert(set_time(0, 0, 0));
	assert(set_time(23, 59, 59));
	assert(set_time(12, 30, 0));
}

void test_timer_parse_time() {
	assert(parse_time("120000"));
	assert(parse_time("000000"));
	assert(parse_time("235959"));
}
