#include "system.h"
#include "literals.h"
#include "serial.h"
#include "timer.h"

__attribute__((weak)) const struct Symbol symbol_table[1] = {{0, "unknown", "unknown", 0}};
__attribute__((weak)) const size_t symbol_count = 0;

void initialize(void) {
	*UART_LCR = 0x03;
	*UART_FCR = 0x07;
	*UART_MCR = 0x00;
	PLIC_PRIORITY[UART_IRQ] = 1;
	*PLIC_ENABLE = 0;
	*PLIC_ENABLE = (1u << UART_IRQ);
	*PLIC_THRESHOLD = 0;
	*UART_IER = 0x1;

	schedule_time(TIMER_INTERVAL);

	__asm__ volatile("li t0, 0x200 \n"
					 "csrs sie, t0 \n"
					 "csrsi sstatus, 0x2 \n"
					 "li t0, 0x20 \n"
					 "csrs sie, t0");
}

void handle_interrupt(const uint64_t cause, const uint64_t epc) {
	switch (cause & CAUSE_CODE) {
		case INTERRUPTION_SOFTWARE: handle_interrupt_software(); return;
		case INTERRUPTION_TIMER: handle_interrupt_timer(); return;
		case INTERRUPTION_EXTERNAL: handle_interrupt_external(); return;
		default: print_exception(cause, epc);
	}
}

void handle_interrupt_software() { __asm__ volatile("csrc sip, %0" ::"r"(2)); }

void handle_interrupt_timer() {
	schedule_time(TIMER_INTERVAL);
	update_time();
}

void handle_interrupt_external() {
	const uint32_t claim = *PLIC_CLAIM;

	while ((*UART_IIR & 0x1) == 0) {
		uart_byte = (uint8_t) *UART;
		uart_byte_ready = 1;
	}

	*PLIC_CLAIM = claim;
}

__attribute__((naked, aligned(4))) void handler(void) {
	__asm__ volatile("addi sp, sp, -144 \n"
					 "csrr t0, sepc \n"
					 "sd t0, 136(sp) \n"
					 "sd s0, 128(sp) \n"
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
					 "addi s0, sp, 144 \n"
					 "csrr a0, scause \n"
					 "ld a1, 136(sp) \n"
					 "call handle \n"
					 "ld ra, 0(sp) \n"
					 "ld t0, 8(sp) \n"
					 "ld t1, 16(sp) \n"
					 "ld t2, 24(sp) \n"
					 "ld s0, 32(sp) \n"
					 "ld a0, 40(sp) \n"
					 "ld a1, 48(sp) \n"
					 "ld a2, 56(sp) \n"
					 "ld a3, 64(sp) \n"
					 "ld a4, 72(sp) \n"
					 "ld a5, 80(sp) \n"
					 "ld a6, 88(sp) \n"
					 "ld a7, 96(sp) \n"
					 "ld t3, 104(sp) \n"
					 "ld t4, 112(sp) \n"
					 "ld t5, 120(sp) \n"
					 "addi sp, sp, 144 \n"
					 "sret \n");
}

__attribute__((noinline)) void throw_exception(void) { __asm__ volatile("ebreak"); }

void print_exception(const uint64_t cause, uint64_t epc) {
	uint64_t frame_now;
	uint64_t frame_next;
	uint64_t address;

	__asm__ volatile("mv %0, s0" : "=r"(frame_now));

	print(cause & CAUSE_INTERRUPTION ? ERROR_INTERRUPTION : ERROR_EXCEPTION);
	print(cause & CAUSE_INTERRUPTION ? ERROR_INTERRUPTIONS[cause & CAUSE_CODE] : ERROR_EXCEPTIONS[cause & CAUSE_CODE]);
	print_line(":");
	print_fault(get_symbol(epc));

	for (int16_t depth = -3; depth < 16 && frame_now != 0; depth++) {
		frame_next = *(uint64_t *) (frame_now - 16);
		address = *(uint64_t *) (frame_now - 8);

		if (address < 0x80200000 || address > 0x80400000) { break; }
		if (depth >= 0) { print_fault(get_symbol(address)); }

		frame_now = frame_next;
	}
}

void print_fault(const struct Symbol * symbol) {
	if (symbol) {
		print(" at ");
		print(symbol->name);
		print(" (");
		print(symbol->file);
		print(":");
		print_num(symbol->line, 10);
		print_line(")");
	}
}

const struct Symbol * get_symbol(const uint64_t address) {
	const struct Symbol * found = NULL;

	for (size_t i = 0; i < symbol_count; i++) {
		if (symbol_table[i].address > address) { break; }
		found = &symbol_table[i];
	}

	return found;
}

void exit(uint32_t code) {
	if (code == 0) { *SIFIVE = SIFIVE_PASS; }
	else { *SIFIVE = code << 16 | SIFIVE_FAIL; }
	while (true) { __asm__ volatile("wfi"); }
}