#include "system.h"
#include "serial.h"

void exit(uint32_t code) {
	if (code == 0) { *SIFIVE = SIFIVE_PASS; }
	else { *SIFIVE = code << 16 | SIFIVE_FAIL; }

	while (true) { __asm__ volatile("wfi"); }
}

__attribute__((naked, aligned(4))) void handler(void) {
	__asm__ volatile("call handle\n"
					 "1: j 1b\n");
}