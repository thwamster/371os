#include "main.h"
#include "serial.h"
#include "system.h"
#include "terminal.h"

int main() { terminal(); }

void handle(void) {
	print_line("Error. Exiting.");
	exit(1);
}