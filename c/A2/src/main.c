#include "main.h"
#include "serial.h"
#include "system.h"
#include "terminal.h"

int main() {
	interrupt_initialize();
	print_line("");
	terminal();
}

void handle(void) {
	uint64_t cause;
	__asm__ volatile("csrr %0, scause" : "=r"(cause));

	if (cause & CAUSE_INTERRUPT) {
		if ((cause & CAUSE_CODE) == 9) { interrupt_handle(); }
		return;
	}

	exit(get_exception());
}
