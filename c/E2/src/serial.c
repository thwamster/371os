#include "serial.h"
#include "exception.h"
#include "layout.h"
#include "string.h"
#include "structures.h"

volatile char inbox[INBOX_SIZE];
volatile size_t inbox_first = 0;
volatile size_t inbox_last = 0;

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

		if (c == ANSI_ESCAPE) {
			if (read_char() == '[') {
				const char direction = read_char();

				if (direction == BACKWARD) {
					if (now > 0) {
						now--;
						cursor_move(BACKWARD, 1);
					}
				}
				else if (direction == FORWARD) {
					if (now < max) {
						now++;
						cursor_move(FORWARD, 1);
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

		if (max - now != 0) { cursor_move(BACKWARD, max - now); }
		cursor_visibility(true);
	}
}

void read_string(char * buffer, const size_t length, const char terminate) {
	if (length == 0) { return; }
	size_t end = 0;

	for (size_t i = 0; i < length - 1; i++) {
		const char c = read_char();
		if (c == terminate) { break; }
		buffer[i] = c;
		end = i;
	}

	buffer[end] = '\0';
}

void read_inbox(char * buffer, const size_t max_length) {
	size_t i = 0;

	while (inbox_first != inbox_last && i < max_length - 1) {
		buffer[i] = read_char();
		i++;
	}

	buffer[i] = '\0';
}

char read_char() {
	while (inbox_first == inbox_last) { __asm__ volatile("wfi"); }

	const char byte = inbox[inbox_last];
	inbox_last = (inbox_last + 1) % INBOX_SIZE;

	return byte;
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

void print_num(const int64_t number) { print_num_ext(number, 10, 0, false); }

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

		*--string = ALPHANUMERIC[-remainder];
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

void format(const char * string) {
	print_char(ANSI_ESCAPE);
	print_char('[');
	print(string);
}

void format_style(const uint8_t code) {
	format("");
	print_num(code);
	print("m");
}

void format_reset() { format_style(RESET); }

void format_rgb(const uint8_t code, const uint8_t red, const uint8_t green, const uint8_t blue) {
	format("");
	print_num(code + RGB);
	print(";2;");
	print_num(red);
	print(";");
	print_num(green);
	print(";");
	print_num(blue);
	print("m");
}

void format_resize(const size_t x, const size_t y) {
	format("8;");
	print_num((int64_t) y);
	print_char(';');
	print_num((int64_t) x);
	print_char('t');

	format_clear();
}

void format_clear() {
	cursor_visibility(false);
	format("2J");
	format("H");
	cursor_visibility(true);
}

void cursor_visibility(const bool visible) { format(visible ? "?25h" : "?25l"); }

void cursor_move(const char direction, const uint8_t number) {
	format("");
	print_num(number);
	print_char(direction);
}

void cursor_position(const size_t x, const size_t y) {
	format("");
	print_num((int64_t) y);
	print_char(';');
	print_num((int64_t) x);
	print_char('H');
}

struct Position query_cursor() {
	format("6n");
	return parse_position('R', '[', ';');
}

struct Position query_size() {
	format("18t");
	return parse_position('t', ';', ';');
}

struct Position parse_position(const char terminate, const char delimiter_first, const char delimiter_second) {
	char buffer[MAX_INPUT];
	read_string(buffer, MAX_INPUT, terminate);

	const char * y = string_find_char(buffer, delimiter_first) + 1;
	const char * x = string_find_char(y, delimiter_second);

	return (struct Position) {(int32_t) string_to_num(x + 1, 10), (int32_t) string_to_num_ext(y, 10, (size_t) (x - y))};
}