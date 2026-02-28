#include "library.h"

void exit(int code) {
	*SIFIVE = code << 16 | 0x5555;

	while (true) { __asm__ volatile("wfi"); }
}

void *memset(void *dest, register int val, register size_t len) {
	register unsigned char *ptr = (unsigned char *) dest;
	while (len-- > 0) *ptr++ = val;
	return dest;
}

int strcmp(const char *string1, const char *string2) {
	unsigned char character1, character2;

	do {
		character1 = (unsigned char) *string1++;
		character2 = (unsigned char) *string2++;
		if (character1 == '\0') return character1 - character2;
	}
	while (character1 == character2);

	return character1 - character2;
}