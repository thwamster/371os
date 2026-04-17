#include "string.h"
#include "allocator.h"

size_t string_length(const char * s) {
	size_t length = 0;
	while (s[length] != '\0') { length++; }
	return length;
}

int8_t string_compare(const char * string1, const char * string2) {
	unsigned char character1, character2;

	do {
		character1 = (unsigned char) *string1++;
		character2 = (unsigned char) *string2++;
		if (character1 == '\0') return (int8_t) (character1 - character2);
	}
	while (character1 == character2);

	return (int8_t) (character1 - character2);
}

int64_t string_to_num(const char * string, const uint8_t base) {
	if (!string) { return 0; }
	return string_to_num_ext(string, base, string_length(string));
}

int64_t string_to_num_ext(const char * string, const uint8_t base, const size_t length) {
	int64_t value = 0;
	int8_t sign = 1;
	size_t i = 0;
	uint8_t digit;

	if (base < 2 || base > 16 || length == 0) { return 0; }

	if (string[0] == '-') {
		sign = -1;
		i = 1;
	}

	for (; i < length; i++) {
		const char c = string[i];

		if (c >= '0' && c <= '9') { digit = c - '0'; }
		else if (c >= 'A' && c <= 'F') { digit = c - 'A' + 10; }
		else if (c >= 'a' && c <= 'f') { digit = c - 'a' + 10; }
		else { return 0; }

		if (digit >= base) { return 0; }

		value = value * base + digit;
	}

	return sign * value;
}

char * string_find_char(const char * string, const char character) {
	do {
		if (*string == character) { return (char *) string; }
	}
	while (*string++);

	return 0;
}

void string_clear(char * buffer, const size_t size) { memory_set(buffer, '\0', size); }

bool char_is_empty(const char c) { return c == ' ' || c == '\0' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'; }