#pragma once

#include <stddef.h>
#include <stdint.h>

#define MANUAL_EXCEPTION *(int *) 0 = 0

struct Symbol {
	uint64_t address;
	const char * name;
	const char * file;
	int line;
};

static volatile uint32_t * const PLIC_PRIORITY = (uint32_t *) 0xC000000;
static volatile uint32_t * const PLIC_ENABLE = (uint32_t *) 0xC002080;
static volatile uint32_t * const PLIC_THRESHOLD = (uint32_t *) 0xC201000;
static volatile uint32_t * const PLIC_CLAIM = (uint32_t *) 0xC201004;
static volatile uint8_t * const UART = (uint8_t *) 0x10000000;
static volatile uint8_t * const UART_IER = (uint8_t *) 0x10000001;
static volatile uint8_t * const UART_LCR = (uint8_t *) 0x10000003;
static volatile uint8_t * const UART_FCR = (uint8_t *) 0x10000002;
static volatile uint8_t * const UART_IIR = (uint8_t *) 0x10000002;
static volatile uint8_t * const UART_MCR = (uint8_t *) 0x10000004;
static volatile uint8_t * const UART_LSR = (uint8_t *) 0x10000005;
static volatile uint64_t * const MTIME = (uint64_t *) 0x200BFF8;
static volatile uint64_t * const MTIMECMP = (uint64_t *) 0x2004000;
static volatile uint32_t * const SIFIVE = (uint32_t *) 0x100000;

static const uint64_t TIMER_INTERVAL = 10000000;
static const uint32_t UART_IRQ = 10;
static const uint32_t SIFIVE_PASS = 0x5555;
static const uint32_t SIFIVE_FAIL = 0x3333;
static const uint64_t CAUSE_INTERRUPTION = 1ULL << 63;
static const uint64_t CAUSE_CODE = ~(1ULL << 63);

enum Interruption {
	INTERRUPTION_SOFTWARE = 1,
	INTERRUPTION_TIMER = 5,
	INTERRUPTION_EXTERNAL = 9,
	INTERRUPTION_COUNTER = 13,
};

enum Exception {
	EXCEPTION_INSTRUCTION_MISALIGNED = 0,
	EXCEPTION_INSTRUCTION_FAULT = 1,
	EXCEPTION_ILLEGAL_INSTRUCTION = 2,
	EXCEPTION_BREAKPOINT = 3,
	EXCEPTION_LOAD_MISALIGNED = 4,
	EXCEPTION_LOAD_FAULT = 5,
	EXCEPTION_STORE_MISALIGNED = 6,
	EXCEPTION_STORE_FAULT = 7,
	EXCEPTION_ECALL_U = 8,
	EXCEPTION_ECALL_S = 9,
	EXCEPTION_INSTRUCTION_PAGE_FAULT = 12,
	EXCEPTION_LOAD_PAGE_FAULT = 13,
	EXCEPTION_STORE_PAGE_FAULT = 15,
	EXCEPTION_SOFTWARE_CHECK = 18,
	EXCEPTION_HARDWARE_ERROR = 19,
};

extern const struct Symbol symbol_table[];
extern const size_t symbol_count;
extern volatile uint8_t uart_byte_ready;
extern volatile uint8_t uart_byte;

void initialize(void);
void handler(void);
void handle_interrupt(uint64_t cause, uint64_t epc);
void handle_interrupt_software(void);
void handle_interrupt_timer(void);
void handle_interrupt_external(void);
void throw_exception(void);
void print_exception(uint64_t cause, uint64_t epc);
void print_fault(const struct Symbol * symbol);
void exit(uint32_t code);
const struct Symbol * get_symbol(uint64_t address);