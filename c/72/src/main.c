#include "main.h"
#include "serial.h"
#include "system.h"
#include "terminal.h"

int main() {
	print_line("");
	terminal();
}

void handle(void) { exit(get_exception()); }