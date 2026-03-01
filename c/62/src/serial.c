#include "serial.h"
#include "library.h"

void read_line(char * buffer) {
	size_t max = 0;
	size_t now = 0;

	while (true) {
		const char c = read_char();

		if (c == '\r') {
			print("\r\n");
			buffer[max] = '\0';
			return;
		}

		if (c == '\x03') {
			print("^C\r\n");
			buffer[0] = '\0';
			return;
		}

		if (c == '\e') {
			if (read_char() == '[') {
				const char direction = read_char();

				if (direction == 'D') {
					if (now > 0) {
						now--;
						cursor_move(LEFT, 1);
					}
				}
				else if (direction == 'C') {
					if (now < max) {
						now++;
						cursor_move(RIGHT, 1);
					}
				}
			}

			continue;
		}

		if (c == '\x7f' || c == '\b') {
			if (now <= 0) { continue; }

			for (int i = (int) now; i <= (int) max; i++) { buffer[i - 1] = buffer[i]; }

			buffer[max] = '\0';
			max -= 1;
			now -= 1;

			cursor_visibility(false);
			print("\b");
			print(&buffer[now]);
			print(" \b");
		}
		else {
			if (max > MAX_INPUT - 1) { continue; }

			for (int i = (int) max; i >= (int) now && i >= 0; i--) { buffer[i + 1] = buffer[i]; }

			buffer[now] = c;
			max++;
			now++;

			cursor_visibility(false);
			print_char(c);
			print(&buffer[now]);
		}

		if (max - now != 0) { cursor_move(LEFT, max - now); }
		cursor_visibility(true);
	}
}

char read_char() {
	while ((*LSR & 0x1) == 0) {}
	return (char) *UART;
}

void print(const char * string) {
	while (*string != '\0') {
		print_char(*string);
		string++;
	}
}

void print_reset(const char * string) {
	print(string);
	format_reset();
}

void print_line(const char * string) {
	print(string);
	print("\r\n");
}

void print_char(const char character) { *UART = character; }

void print_int(const int number, const int base) {
	char buffer[MAX_INPUT];
	char * string = &buffer[sizeof buffer - 1];

	if (base < 2 || base > 16) { return; }

	*string = '\0';

	int value = number < 0 ? number : -number;
	do {
		const int quotient = value / base;
		const int remainder = value % base;
		*(--string) = "0123456789ABCDEF"[-remainder];
		value = quotient;
	}
	while (value);

	if (number < 0) { *(--string) = '-'; }

	print(string);
}

void format(const uint8_t code) {
	print("\e[");
	print_int(code, 10);
	print("m");
}

void format_reset() { format(RESET); }

void format_rgb(const uint8_t code, const uint8_t red, const uint8_t green, const uint8_t blue) {
	print("\e[");
	print_int(code + RGB, 10);
	print(";2;");
	print_int(red, 10);
	print(";");
	print_int(green, 10);
	print(";");
	print_int(blue, 10);
	print("m");
}

void format_clear() {
	cursor_visibility(false);
	print("\e[2J\e[H");
	cursor_visibility(true);
}

void cursor_visibility(const bool visible) { print(visible ? "\x1b[?25h" : "\x1b[?25l"); }

void cursor_move(const char direction, const uint8_t number) {
	print("\e[");
	print_int(number, 10);
	print_char(direction);
}