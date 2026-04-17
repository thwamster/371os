#include "random.h"

bool random_seed = false;
uint64_t state = 0x0123456789ABCDEF;

void random_initialize() {
	uint64_t cycles;

	__asm__ volatile("rdcycle %0" : "=r"(cycles));

	state = cycles == 0 ? 1 : cycles;
	random_seed = true;
}

uint64_t random_num() {
	uint64_t x = state;

	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;

	state = x;
	return x;
}

uint32_t random_range(const uint32_t min, const uint32_t max) {
	if (min >= max) { return min; }
	return min + random_num() % (max - min + 1);
}