#include "main.h"
#include "../src/serial.h"
#include "../src/system.h"
#include "tests.h"

int main(void) {
	if (test_index == 0) { print_line("Initiating testing."); }

	while (test_index < test_count) {
		print("Running test ");
		print_int((int) test_index + 1, 10);
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
	print_int((int) (test_count - fail_count), 10);
	print_char('/');
	print_int((int) test_count, 10);
	print_line(".");
	exit(fail_count);
}

void handle(void) {
	uint64_t sepc;

	if (tests[test_index].expecting_pass) {
		print("UNEXPECTED ");
		get_exception();
		test_fail();
	}
	else {
		print("EXPECTED ");
		get_exception();
		test_pass();
	}

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