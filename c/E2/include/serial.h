#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum Format { RESET = 0, BOLD = 1, FAINT = 2, ITALIC = 3, UNDERLINE = 4, BLINK_SLOW = 5, BLINK_RAPID = 6, INVERT = 7, CONCEAL = 8, STRIKE = 9 };
enum Color { BLACK = 0, RED = 1, GREEN = 2, YELLOW = 3, BLUE = 4, MAGENTA = 5, CYAN = 6, WHITE = 7, RGB = 8 };
enum Navigation { ABOVE = 'A', BELOW = 'B', FORWARD = 'C', BACKWARD = 'D', NEXT = 'E', PREVIOUS = 'F' };

static const char ANSI_ESCAPE = '\x1B';
static const char * const ALPHANUMERIC = "0123456789ABCDEF";
static const size_t MAX_INPUT = 0x80;
static const uint8_t FORMAT_NOT = 20;
static const uint8_t COLOR_FOREGROUND = 30;
static const uint8_t COLOR_BACKGROUND = 40;
static const uint8_t COLOR_BRIGHT = 60;
enum { INBOX_SIZE = 0x80 };

extern volatile char inbox[];
extern volatile size_t inbox_first;
extern volatile size_t inbox_last;

void read_line(char * buffer);
void read_string(char * buffer, size_t length, char terminate);
void read_inbox(char * buffer, size_t max_length);
char read_char(void);
void print(const char * string);
void print_reset(const char * string);
void print_line(const char * string);
void print_char(char character);
void print_num(int64_t number);
void print_num_ext(int64_t number, uint8_t base, bool prefix, uint8_t length);
void format(const char * string);
void format_style(uint8_t code);
void format_reset(void);
void format_rgb(uint8_t code, uint8_t red, uint8_t green, uint8_t blue);
void format_resize(size_t x, size_t y);
void format_clear(void);
void cursor_visibility(bool visible);
void cursor_move(char direction, uint8_t number);
void cursor_position(size_t x, size_t y);
struct Position query_cursor();
struct Position query_size();
struct Position parse_position(char terminate, char delimiter_first, char delimiter_second);