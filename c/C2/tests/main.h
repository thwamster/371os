#pragma once

#include <stdbool.h>
#include <stdint.h>

extern bool initialized;

int main(void);
void handle(uint64_t cause, uint64_t epc);
void test_pass(void);
void test_fail(void);