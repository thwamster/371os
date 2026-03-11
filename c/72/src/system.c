#include "system.h"

#include "literals.h"
#include "serial.h"

void exit(uint32_t code) {
	if (code == 0) { *SIFIVE = SIFIVE_PASS; }
	else { *SIFIVE = code << 16 | SIFIVE_FAIL; }
	while (true) { __asm__ volatile("wfi"); }
}

__attribute__((naked, aligned(4))) void handler(void) {
	__asm__ volatile("addi sp, sp, -136 \n"
					 "sd ra, 0(sp) \n"
					 "sd t0, 8(sp) \n"
					 "sd t1, 16(sp) \n"
					 "sd t2, 24(sp) \n"
					 "sd s0, 32(sp) \n"
					 "sd a0, 40(sp) \n"
					 "sd a1, 48(sp) \n"
					 "sd a2, 56(sp) \n"
					 "sd a3, 64(sp) \n"
					 "sd a4, 72(sp) \n"
					 "sd a5, 80(sp) \n"
					 "sd a6, 88(sp) \n"
					 "sd a7, 96(sp) \n"
					 "sd t3, 104(sp) \n"
					 "sd t4, 112(sp) \n"
					 "sd t5, 120(sp) \n"
					 "sd t6, 128(sp) \n"
					 "ld s0, 32(sp) \n"
					 "call handle \n"
					 "1: j 1b \n");
}

void throw_exception(void) { __asm__ volatile("ebreak"); }

uint64_t get_exception(void) {
	uint64_t cause;
	uint64_t code;
	uint64_t frame_now;
	uint64_t frame_next;
	uint64_t address;

	__asm__ volatile("csrr %0, scause" : "=r"(cause));
	__asm__ volatile("mv %0, s0" : "=r"(frame_now));

	code = cause & CAUSE_CODE;

	if (cause & CAUSE_INTERRUPT) {
		print(ERROR_INTERRUPTION);
		print_line(ERROR_INTERRUPTIONS[code]);
	}
	else {
		print(ERROR_EXCEPTION);
		print_line(ERROR_EXCEPTIONS[code]);
	}

	for (int depth = 0; depth < 16 && frame_now != 0; depth++) {
		address = *(uint64_t *) (frame_now - 8);
		frame_next = *(uint64_t *) (frame_now - 16);

		print(" AT ");
		print_int((int) address, 16);
		print_line("");

		if (frame_next <= frame_now) { break; }
		frame_now = frame_next;
	}

	return cause;
}