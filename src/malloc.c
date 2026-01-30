#include "malloc.h"

int main() {
	const size_t p0 = my_malloc(16);
	const size_t p1 = my_malloc(32);
	const int32_t x = 0x44332211;
	const int32_t y = 0x12345678;
	printf("p0 = %zu\np1 = %zu\n", p0, p1);
	printf("x = %lu\ny = %lu\n", x, y);
	set(x, p0);
	set(y, p1);
	const int32_t z = get(p0);
	const int32_t w = get(p1);
	printf("z = %lu\nw = %lu\n", z, w);
	assert(x == z);
	printf("x == z, %lu == %lu\n", x, z);
	assert(y == w);
	printf("y == w, %lu == %lu\n", y, w);
	assert(my_malloc(2048) == (size_t) -1);
	printf("my_malloc(2048) == (size_t) -1, %llu == %llu\n", my_malloc(2048), (size_t) -1);
	assert(my_malloc(32) != (size_t) -1);
	printf("my_malloc(32) != (size_t) -1, %llu != %llu\n", my_malloc(32), (size_t) -1);
	assert(my_malloc(64) == (size_t) -1);
	printf("my_malloc(64) == (size_t) -1, %llu == %llu\n", my_malloc(64), (size_t) -1);
}

void init() {
	assert((SIZE & (SIZE - 1)) == 0);

	for (size_t i = 0; i < SIZE >> 6; i++) {
		BUS[i] = 0xFF;
	}
}

size_t my_malloc(const size_t s) {
	assert(s > 0);

	if (BUS[0] == 0) {
		init();
	}

	size_t c = 0;
	size_t m = 0;

	for (size_t i = 0; i < SIZE; i++) {
		if (c >= s) {
			break;
		}

		if ((BUS[i / 8] & (1 << i % 8)) == 0) {
			c += 1;
			continue;
		}

		c = 0;
		m = i + 1;
	}

	if (c >= s) {
		for (size_t i = m; i < m + s; i++) {
			BUS[i / 8] |= 1 << i % 8;
		}
		return m;
	}

	return (size_t) -1;
}

void set(const int32_t n, const size_t s) {
	BUS[s + 0] = n;
	BUS[s + 1] = n >> 8 & 0xFF;
	BUS[s + 2] = n >> 16 & 0xFF;
	BUS[s + 3] = n >> 24 & 0xFF;
}

int32_t get(const size_t s) { return BUS[s + 0] | BUS[s + 1] << 8 | BUS[s + 2] << 16 | BUS[s + 3] << 24; }

void print() {
	for (size_t i = 0; i < SIZE; i++) {
		if (i % 8 == 0 && i != 0) {
			printf("\n");
		}

		printf("%lu ", BUS[i]);
	}

	printf("\n\n");
}
