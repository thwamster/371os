#include "library.h"

void exit(uint32_t code) {
	*SIFIVE = code << 16 | 0x5555;

	while (true) { __asm__ volatile("wfi"); }
}

void * memset(void * dest, register int val, register size_t len) {
	register unsigned char * ptr = (unsigned char *) dest;
	while (len-- > 0) *ptr++ = val;
	return dest;
}

int strcmp(const char * string1, const char * string2) {
	unsigned char character1, character2;

	do {
		character1 = (unsigned char) *string1++;
		character2 = (unsigned char) *string2++;
		if (character1 == '\0') return character1 - character2;
	}
	while (character1 == character2);

	return character1 - character2;
}

bool is_empty(const char c) { return c == ' ' || c == '\0' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'; }

int parse_int(const char * string) {
	int value = 0;
	int sign = 1;

	if (*string == '-') {
		sign = -1;
		string++;
	}

	while (*string != '\0') {
		if (*string < '0' || *string > '9') { return 0; }

		value *= 10;
		value += *string - '0';
		string++;
	}
	return sign * value;
}