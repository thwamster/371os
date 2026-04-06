#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

static const uint32_t UART_IRQ = 10;
static const size_t MAX_INPUT = 0x80;
static const uint8_t FORMAT_NOT = 20;
static const uint8_t COLOR_FOREGROUND = 30;
static const uint8_t COLOR_BACKGROUND = 40;
static const uint8_t COLOR_BRIGHT = 60;

static volatile uint8_t uart_byte_ready = 0;
static volatile uint8_t uart_byte = 0;

enum Format { RESET = 0, BOLD = 1, FAINT = 2, ITALIC = 3, UNDERLINE = 4, BLINK_SLOW = 5, BLINK_RAPID = 6, INVERT = 7, CONCEAL = 8, STRIKE = 9 };
enum Color { BLACK = 0, RED = 1, GREEN = 2, YELLOW = 3, BLUE = 4, MAGENTA = 5, CYAN = 6, WHITE = 7, RGB = 8 };
enum Cursor { UP = 'A', DOWN = 'B', RIGHT = 'C', LEFT = 'D', NEXT = 'E', PREVIOUS = 'F' };

void interrupt_initialize(void);
void interrupt_handle(void);
void read_line(char * buffer);
char read_char(void);
void print(const char * string);
void print_reset(const char * string);
void print_line(const char * string);
void print_char(char character);
void print_int(int number, int base);
void format(uint8_t code);
void format_reset(void);
void format_rgb(uint8_t code, uint8_t red, uint8_t green, uint8_t blue);
void format_clear();
void cursor_visibility(bool visible);
void cursor_move(char direction, uint8_t number);