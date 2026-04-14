#include "allocator.h"
#include "library.h"

volatile uint64_t next_frame = 0;

void initialize_frame() { next_frame = ((uint64_t) &end + FRAME_SIZE - 1) & ~(uint64_t) (FRAME_SIZE - 1); }

uint64_t allocate_frame() {
	const uint64_t frame = next_frame;

	next_frame += FRAME_SIZE;
	memory_set((void *) frame, 0, FRAME_SIZE);

	return frame;
}