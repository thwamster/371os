#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "structures.h"

enum Direction { UP = 0, LEFT = 1, DOWN = 2, RIGHT = 3 };
enum Dimensions { WIDTH = 10, HEIGHT = 9, PATH = WIDTH * HEIGHT };
enum Board {
	BLANK = 0,
	APPLE = 1,
	SNAKE_N = 2,
	SNAKE_W = 3,
	SNAKE_S = 4,
	SNAKE_E = 5,
	SNAKE_V = 6,
	SNAKE_H = 7,
	SNAKE_NW = 8,
	SNAKE_NE = 9,
	SNAKE_SW = 10,
	SNAKE_SE = 11
};
enum Action { GAME_NONE = 0, GAME_MOVEMENT = 1, GAME_PAUSE = 2, GAME_RESTART = 3, GAME_EXIT = 4 };
enum Outcome { LOSS = 0, NONE = 1, WIN = 2 };

static const struct Position SET_SNAKE[] = {{0, 4}, {1, 4}, {2, 4}};
static const struct Position SET_APPLE[] = {{.x = 8, .y = 4}};
static const struct Position DIRECTIONS[] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
static const uint16_t DELAY_PAUSE = 300;
static const uint16_t DELAY_MAXIMUM = 200;
static const uint16_t DELAY_MINIMUM = 150;
static const uint8_t GAME_TABLE[4][4] = {
	[UP] = {SNAKE_V, SNAKE_NE, SNAKE_V, SNAKE_NW},
	[LEFT] = {SNAKE_SW, SNAKE_H, SNAKE_NW, SNAKE_H},
	[DOWN] = {SNAKE_V, SNAKE_SE, SNAKE_V, SNAKE_SW},
	[RIGHT] = {SNAKE_SE, SNAKE_H, SNAKE_NE, SNAKE_H},
};

extern struct Position snake_path[PATH];
extern enum Direction snake_direction;
extern enum Direction snake_momentum;
extern size_t snake_head;
extern size_t snake_tail;
extern uint8_t game_board[HEIGHT][WIDTH];
extern bool game_started;
extern bool game_paused;

void game_initialize(void);
void game_initialize_board();
void game_run(void);
enum Action action(void);
enum Action action_movement(char character);
enum Outcome move(void);
void move_head(struct Position head, struct Position new_head);
void move_tail(struct Position tail);
void move_apple(void);
void draw(void);
void draw_square(size_t x, size_t y);
void draw_square_wall(size_t x, size_t y);
void draw_square_board(size_t x, size_t y);
void window_open(void);
void window_close(struct Position original_size);