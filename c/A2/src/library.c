#include "library.h"

void * memory_set(void * dest, register uint8_t val, register size_t len) {
	register unsigned char * ptr = (unsigned char *) dest;
	while (len-- > 0) *ptr++ = val;
	return dest;
}

size_t str_len(const char * s) {
	size_t length = 0;
	while (s[length] != '\0') { length++; }
	return length;
}

int8_t str_cmp(const char * string1, const char * string2) {
	unsigned char character1, character2;

	do {
		character1 = (unsigned char) *string1++;
		character2 = (unsigned char) *string2++;
		if (character1 == '\0') return (int8_t) (character1 - character2);
	}
	while (character1 == character2);

	return (int8_t) (character1 - character2);
}

int64_t str_to_num(const char * string, const uint8_t base) {
	if (!string) { return 0; }
	return str_to_num_len(string, base, str_len(string));
}

int64_t str_to_num_len(const char * string, const uint8_t base, const size_t length) {
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

void str_clear(char * buffer, const size_t size) { memory_set(buffer, '\0', size); }

bool char_is_empty(const char c) { return c == ' ' || c == '\0' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'; }