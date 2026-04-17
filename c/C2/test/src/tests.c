#include "tests.h"
#include "allocator.h"
#include "exception.h"
#include "layout.h"
#include "literals.h"
#include "paging.h"
#include "serial.h"
#include "string.h"
#include "terminal.h"
#include "timer.h"

struct Test tests[] = {{test_allocator_align_frame, true},
					   {test_allocator_align_size, true},
					   {test_allocator_memory_copy, true},
					   {test_allocator_memory_move, true},
					   {test_exception_error_handler, false},
					   {test_exception_get_symbol, true},
					   {test_exception_manual, false},
					   {test_exception_plic, true},
					   {test_paging_align_address, true},
					   {test_paging_pa_to_ppn, true},
					   {test_paging_ppn_to_pa, true},
					   {test_serial_cursor, true},
					   {test_serial_format, true},
					   {test_serial_print, true},
					   {test_serial_print_num_bases, true},
					   {test_string_char_is_empty, true},
					   {test_string_clear, true},
					   {test_string_compare, true},
					   {test_string_length, true},
					   {test_string_memory_set, true},
					   {test_string_to_num, true},
					   {test_string_to_num_ext, true},
					   {test_terminal_parse_arguments, true},
					   {test_terminal_parse_command, true},
					   {test_timer_parse_time, true},
					   {test_timer_set_time, true},
					   {test_timer_update_time, true}};

size_t test_index = 0;
size_t test_count = sizeof(tests) / sizeof(tests[0]);
uint32_t fail_count = 0;

void test_pass(void) { print_line(TEST_PASS); }

void test_fail(void) {
	print_line(TEST_FAIL);
	fail_count++;
}

void test_allocator_align_frame() {
	assert(align_frame(0x1000) == 0x1000);
	assert(align_frame(0x1001) == 0x2000);
}

void test_allocator_align_size() {
	assert(align_size(1) >= 1);
	assert(align_size(16) >= 16);
}

void test_allocator_memory_copy() {
	const char src[] = "test";
	char dst[5];
	memory_copy(dst, src, 5);
	assert(dst[0] == 't' && dst[1] == 'e' && dst[2] == 's' && dst[3] == 't' && dst[4] == '\0');
}

void test_allocator_memory_move() {
	char buf[] = "12345";
	memory_move(buf + 1, buf, 4);
	assert(buf[0] == '1' && buf[1] == '1' && buf[2] == '2' && buf[3] == '3' && buf[4] == '4');
}

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

void test_exception_manual() { throw_exception(); }

void test_exception_plic() {
	const volatile uint32_t priority = *PLIC_PRIORITY;
	const volatile uint32_t enable = *PLIC_ENABLE;
	const volatile uint32_t threshold = *PLIC_THRESHOLD;
	(void) priority;
	(void) enable;
	(void) threshold;
}

void test_paging_align_address() {
	assert(align_address((const volatile void *) 0x1000) == 0x1000);
	assert(align_address((const volatile void *) 0x1001) == 0x1000);
	assert(align_address((const volatile void *) 0x1FFF) == 0x1000);
}

void test_paging_pa_to_ppn() { assert(pa_to_ppn(0x80000000) == (0x80000000 >> PAGE_SHIFT)); }

void test_paging_ppn_to_pa() { assert(ppn_to_pa(0x80000) == (0x80000ULL << PAGE_SHIFT)); }

void test_serial_cursor() {
	cursor_visibility(false);
	cursor_move(RIGHT, 1);
	cursor_move(LEFT, 1);
	cursor_move(CURSOR_UP, 1);
	cursor_move(CURSOR_DOWN, 1);
	cursor_visibility(true);
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

void test_string_char_is_empty() {
	assert(char_is_empty(' '));
	assert(char_is_empty('\t'));
	assert(char_is_empty('\n'));
	assert(char_is_empty('\r'));
	assert(!char_is_empty('a'));
	assert(!char_is_empty('0'));
}

void test_string_clear() {
	char buf[8];
	memory_set(buf, 0xFF, sizeof(buf));
	string_clear(buf, sizeof(buf));
	for (size_t i = 0; i < sizeof(buf); i++) { assert(buf[i] == '\0'); }
}

void test_string_compare() {
	assert(string_compare("0", "0") == 0);
	assert(string_compare("0", "1") != 0);
}

void test_string_length() {
	assert(string_length("") == 0);
	assert(string_length("hello") == 5);
	assert(string_length("0") == 1);
}

void test_string_memory_set() {
	uint8_t buf[16];
	memory_set(buf, 0xAB, sizeof(buf));
	for (size_t i = 0; i < sizeof(buf); i++) { assert(buf[i] == 0xAB); }
	memory_set(buf, 0x00, sizeof(buf));
	for (size_t i = 0; i < sizeof(buf); i++) { assert(buf[i] == 0x00); }
}

void test_string_to_num() {
	assert(string_to_num("0", 10) == 0);
	assert(string_to_num("1", 10) == 1);
	assert(string_to_num("255", 10) == 255);
	assert(string_to_num("ff", 16) == 255);
	assert(string_to_num("FF", 16) == 255);
	assert(string_to_num("11111111", 2) == 255);
	assert(string_to_num("-1", 10) == -1);
}

void test_string_to_num_ext() {
	assert(string_to_num_ext("12345", 10, 3) == 123);
	assert(string_to_num_ext("FF00", 16, 2) == 255);
}

void test_terminal_parse_arguments() {
	char args[MAX_ARGUMENTS][MAX_ARGUMENT_LENGTH];
	const size_t count = parse_arguments(args, "cmd arg1 arg2");
	assert(count == 3);
	assert(string_compare(args[0], "cmd") == 0);
	assert(string_compare(args[1], "arg1") == 0);
	assert(string_compare(args[2], "arg2") == 0);
}

void test_terminal_parse_command() {
	assert(parse_command("exit") == EXIT);
	assert(parse_command("help") == HELP);
	assert(parse_command("clear") == CLEAR);
	assert(parse_command("time") == TIME);
	assert(parse_command("exception") == EXCEPTION);
}

void test_timer_parse_time() {
	assert(parse_time("120000"));
	assert(parse_time("000000"));
	assert(parse_time("235959"));
}

void test_timer_set_time() {
	assert(set_time(0, 0, 0));
	assert(set_time(23, 59, 59));
	assert(set_time(12, 30, 0));
}

void test_timer_update_time() {
	set_time(23, 59, 59);
	update_time();
	assert(clock.hours == 0 && clock.minutes == 0 && clock.seconds == 0);
}