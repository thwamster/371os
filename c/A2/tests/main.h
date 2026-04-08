#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define TEST_PASS "Test Result: PASS."
#define TEST_FAIL "Test Result: FAIL."

int main(void);
void handle(uint64_t cause, uint64_t epc);
void test_pass(void);
void test_fail(void);