#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static volatile unsigned char * UART = (unsigned char *) 0x10000000;
static volatile unsigned char * LSR = (unsigned char *) 0x10000005;

static const size_t MAX_INPUT = 0x80;
static const uint8_t FORMAT_NOT = 20;
static const uint8_t COLOR_FOREGROUND = 30;
static const uint8_t COLOR_BACKGROUND = 40;
static const uint8_t COLOR_BRIGHT = 60;

enum Format { RESET = 0, BOLD = 1, FAINT = 2, ITALIC = 3, UNDERLINE = 4, BLINK_SLOW = 5, BLINK_RAPID = 6, INVERT = 7, CONCEAL = 8, STRIKE = 9 };
enum Color { BLACK = 0, RED = 1, GREEN = 2, YELLOW = 3, BLUE = 4, MAGENTA = 5, CYAN = 6, WHITE = 7, RGB = 8 };
enum Cursor { UP = 'A', DOWN = 'B', RIGHT = 'C', LEFT = 'D', NEXT = 'E', PREVIOUS = 'F' };

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