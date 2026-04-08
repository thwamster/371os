#include "serial.h"
#include "library.h"
#include "system.h"

volatile uint8_t uart_byte_ready = 0;
volatile uint8_t uart_byte = 0;

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

			for (size_t i = now; i <= max; i++) { buffer[i - 1] = buffer[i]; }

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

			for (int16_t i = (int16_t) max; i >= (int16_t) now && i >= 0; i--) { buffer[i + 1] = buffer[i]; }

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

char read_char(void) {
	while (!uart_byte_ready) { __asm__ volatile("wfi"); }
	uart_byte_ready = 0;

	return (char) uart_byte;
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

void print_num(const int64_t number, const uint8_t base) { print_num_ext(number, base, 0, false); }

void print_num_ext(const int64_t number, const uint8_t base, const bool prefix, const uint8_t length) {
	char buffer[MAX_INPUT];
	char * string = &buffer[sizeof buffer - 1];
	uint8_t current_length = 0;

	if (base < 2 || base > 16) { return; }

	*string = '\0';

	int64_t value = number < 0 ? number : -number;
	do {
		const int64_t quotient = value / base;
		const int64_t remainder = value % base;

		*--string = "0123456789ABCDEF"[-remainder];
		current_length++;
		value = quotient;
	}
	while (value);

	while (current_length < length) {
		*--string = '0';
		current_length++;
	}

	if (prefix) {
		char prefix_char = '\0';

		switch (base) {
			case 2: prefix_char = 'b'; break;
			case 8: prefix_char = 'o'; break;
			case 10: prefix_char = 'd'; break;
			case 16: prefix_char = 'x'; break;
			default:;
		}

		if (prefix_char != '\0') {
			*--string = prefix_char;
			*--string = '0';
		}
	}

	if (number < 0) { *--string = '-'; }

	print(string);
}

void format(const uint8_t code) {
	print("\e[");
	print_num(code, 10);
	print("m");
}

void format_reset() { format(RESET); }

void format_rgb(const uint8_t code, const uint8_t red, const uint8_t green, const uint8_t blue) {
	print("\e[");
	print_num(code + RGB, 10);
	print(";2;");
	print_num(red, 10);
	print(";");
	print_num(green, 10);
	print(";");
	print_num(blue, 10);
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
	print_num(number, 10);
	print_char(direction);
}