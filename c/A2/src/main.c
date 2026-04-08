#include "main.h"
#include "serial.h"
#include "system.h"
#include "terminal.h"

int main() {
	initialize();
	print_line("");
	terminal();
}

void handle(const uint64_t cause, const uint64_t epc) {
	if (cause & CAUSE_INTERRUPTION) {
		handle_interrupt(cause, epc);
		return;
	}

	print_exception(cause, epc);
	exit(cause & CAUSE_CODE);
}