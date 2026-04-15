#include "allocator.h"
#include "exception.h"
#include "layout.h"

volatile uint64_t next_frame = 0;

void frame_initialize() { next_frame = align_frame((uint64_t) &end); }

uint64_t frame_allocate() { return frame_allocate_ext(1); }

uint64_t frame_allocate_ext(const size_t count) {
	const uint64_t frame = next_frame;
	const uint64_t size = count * FRAME_SIZE;

	if (frame + size > KERNEL_END) { return 0; }

	next_frame += size;
	memory_set((void *) frame, 0, size);

	return frame;
}

static struct Block * free_list = 0;

void * memory_allocate(size_t size) {
	if (size == 0) { return 0; }

	size = align_size(size);

	struct Block * prev = 0;
	struct Block * now = free_list;

	while (now != 0) {
		if (now->size >= size) {
			if (now->size > size + 2 * BLOCK_SIZE) {
				struct Block * block = (struct Block *) ((char *) (now + 1) + size);

				block->next = now->next;
				block->size = now->size - size - BLOCK_SIZE;

				now->next = block;
				now->size = size;
			}

			if (prev == 0) { free_list = now->next; }
			else { prev->next = now->next; }

			return now + 1;
		}

		prev = now;
		now = now->next;
	}

	const size_t count = (size + BLOCK_SIZE + FRAME_SIZE - 1) / FRAME_SIZE;
	struct Block * block = (struct Block *) frame_allocate_ext(count);

	if (block == 0) { return 0; }

	block->next = 0;
	block->size = count * FRAME_SIZE - BLOCK_SIZE;

	if (prev == 0) { free_list = block; }
	else { prev->next = block; }

	return memory_allocate(size);
}

void * memory_reallocate(void * pointer, size_t size_new) {
	if (pointer == 0) { return memory_allocate(size_new); }

	if (size_new == 0) {
		memory_deallocate(pointer);
		return 0;
	}

	const struct Block * block = (struct Block *) pointer - 1;
	const size_t size_old = block->size;

	size_new = align_size(size_new);

	if (size_old >= size_new) { return pointer; }

	void * new_pointer = memory_allocate(size_new);
	if (new_pointer != 0) {
		memory_copy(new_pointer, pointer, size_old);
		memory_deallocate(pointer);
	}

	return new_pointer;
}

void memory_deallocate(void * pointer) {
	if (pointer == 0) { return; }

	struct Block * block = (struct Block *) pointer - 1;
	struct Block * prev = 0;
	struct Block * now = free_list;

	while (now != 0 && now < block) {
		prev = now;
		now = now->next;
	}

	if (prev == 0) {
		block->next = free_list;
		free_list = block;
	}
	else {
		block->next = prev->next;
		prev->next = block;
	}

	if (block->next != 0) {
		if ((char *) (block + 1) + block->size == (char *) block->next) {
			block->size += BLOCK_SIZE + block->next->size;
			block->next = block->next->next;
		}
	}

	if (prev != 0) {
		if ((char *) (prev + 1) + prev->size == (char *) block) {
			prev->size += BLOCK_SIZE + block->size;
			prev->next = block->next;
		}
	}
}

void * memory_set(void * destination, const uint8_t value, size_t size) {
	register unsigned char * ptr = destination;
	while (size-- > 0) *ptr++ = value;
	return destination;
}

void * memory_copy(void * restrict destination, const void * source, const size_t size) {
	char * d = destination;
	const char * s = source;

	for (size_t i = 0; i < size; i++) { d[i] = s[i]; }

	return destination;
}

void * memory_move(void * restrict destination, const void * source, const size_t size) {
	char * d = destination;
	const char * s = source;

	if (d == s || size == 0) return destination;

	if (d < s) {
		for (size_t i = 0; i < size; i++) { d[i] = s[i]; }
	}
	else {
		for (size_t i = size; i > 0; i--) { d[i - 1] = s[i - 1]; }
	}

	return destination;
}

uint64_t align_frame(const uint64_t address) { return (address + FRAME_SIZE - 1) & ~(FRAME_SIZE - 1); }
size_t align_size(const size_t size) { return (size + POINTER_SIZE - 1) & ~(POINTER_SIZE - 1); }