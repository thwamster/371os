#pragma once

#include <stddef.h>
#include <stdint.h>

#include "structures.h"

#include <stdbool.h>

enum Direction { UP = 0, LEFT = 1, DOWN = 2, RIGHT = 3 };
enum Size { WIDTH = 17, HEIGHT = 15 };
enum Snake { MAX_SNAKE_LENGTH = WIDTH * HEIGHT };
enum Board { BLANK = 0, SNAKE = 1, APPLE = 2 };
enum Result { LOSS = 0, NONE = 1, WIN = 2 };

static const struct Position empty_position = {0, 0};
static const struct Position snake_set[] = {{1, 7}, {2, 7}, {3, 7}};
static const struct Position apple_set = {.x = 12, .y = 7};
static const struct Position directions[] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};

extern uint8_t game_board[HEIGHT][WIDTH];
extern struct Position snake_path[MAX_SNAKE_LENGTH];
extern enum Direction snake_direction;
extern enum Direction snake_momentum;
extern size_t snake_head;
extern size_t snake_tail;
extern bool game_on;

void snake_run();
void snake_get_input();
void snake_window_open();
void snake_window_close(struct Position original_size);
void snake_initialize();
enum Result snake_move();
void apple_new();
void draw_board();