#include "main.h"
#include "../src/serial.h"
#include "../src/system.h"
#include "tests.h"

int main(void) {
	if (test_index == 0) { print_line("Initiating testing."); }

	while (test_index < test_count) {
		print("Running test ");
		print_num((int) test_index + 1, 10);
		print(". ");

		tests[test_index].test();

		if (tests[test_index].expecting_pass) {
			print("Expected execution. ");
			test_pass();
		}
		else {
			print("Unexpected execution. ");
			test_fail();
		}

		test_index++;
	}

	print("All tests completed. Tests passed: ");
	print_num(test_count - fail_count, 10);
	print_char('/');
	print_num(test_count, 10);
	print_line(".");
	exit(fail_count);
}

void handle(void) {
	if (tests[test_index].expecting_pass) {
		print("Unexpected error. ");
		test_fail();
	}
	else {
		print("Expected error. ");
		test_pass();
	}

	uint64_t sepc;
	asm volatile("csrr %0, sepc" : "=r"(sepc));
	sepc += 4;
	asm volatile("csrw sepc, %0" ::"r"(sepc));
	test_index++;
	main();
	exit(fail_count);
}

void test_pass(void) { print_line(TEST_PASS); }

void test_fail(void) {
	print_line(TEST_FAIL);
	fail_count++;
}