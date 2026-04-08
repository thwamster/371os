#include "main.h"
#include "../src/literals.h"
#include "../src/serial.h"
#include "../src/system.h"
#include "tests.h"

int main(void) {
	initialize();

	if (test_index == 0) { print_line(TEST_INITIATE); }

	while (test_index < test_count) {
		print(TEST_RUN);
		print_num((int) test_index + 1, 10);
		print(". ");

		tests[test_index].test();

		if (tests[test_index].expecting_pass) {
			print(TEST_EXECUTE_EXPECTED);
			test_pass();
		}
		else {
			print(TEST_EXECUTE_UNEXPECTED);
			test_fail();
		}

		test_index++;
	}

	print(TEST_COMPLETE);
	print_num((int) (test_count - fail_count), 10);
	print_char('/');
	print_num((int) test_count, 10);
	print_line(".");
	exit(fail_count);
}

void handle(const uint64_t cause, const uint64_t epc) {
	if (cause & CAUSE_INTERRUPTION) {
		handle_interrupt(cause, epc);
		return;
	}

	if (tests[test_index].expecting_pass) {
		print_exception(cause, epc);
		print(TEST_EXCEPTION_EXPECTED);
		test_fail();
	}
	else {
		print_exception(cause, epc);
		print(TEST_EXCEPTION_UNEXPECTED);
		test_pass();
	}

	test_index++;
	main();
	exit(fail_count);
}

void test_pass(void) { print_line(TEST_PASS); }

void test_fail(void) {
	print_line(TEST_FAIL);
	fail_count++;
}