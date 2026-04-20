#include "game.h"
#include "allocator.h"
#include "literals.h"
#include "random.h"
#include "serial.h"

#include <string.h>
#include <timer.h>

struct Position snake_path[PATH];
enum Direction snake_direction;
enum Direction snake_momentum;
size_t snake_head = 0;
size_t snake_tail = 0;
uint8_t game_board[HEIGHT][WIDTH];
bool game_started = false;
bool game_paused = false;

void game_initialize() {
	memory_set(game_board, 0, HEIGHT * WIDTH);
	memory_set(snake_path, 0, PATH);
	snake_direction = RIGHT;
	snake_momentum = RIGHT;
	snake_head = sizeof(SET_SNAKE) / POSITION_SIZE - 1;
	snake_tail = 0;
	game_initialize_board();
	game_started = false;
}

void game_initialize_board() {
	for (size_t i = snake_tail; i <= snake_head; i++) {
		snake_path[i] = SET_SNAKE[i];
		game_board[SET_SNAKE[i].y][SET_SNAKE[i].x] = SNAKE_H;
	}

	game_board[SET_SNAKE[snake_head].y][SET_SNAKE[snake_head].x] = SNAKE_E;

	for (size_t i = 0; i < sizeof(SET_APPLE) / POSITION_SIZE; i++) { game_board[SET_APPLE[i].y][SET_APPLE[i].x] = APPLE; }
}

void game_run() {
	const struct Position original_size = query_size();
	window_open();
	game_initialize();
	draw();

	uint16_t delay = DELAY_MAXIMUM;

	while (true) {
		if (game_paused) {
			const enum Action result = action();

			if (result == GAME_PAUSE) {
				game_paused = false;
				continue;
			}

			if (result == GAME_RESTART) {
				game_initialize();
				delay = DELAY_MAXIMUM;
				game_paused = false;
				continue;
			}

			if (result == GAME_EXIT) { break; }

			continue;
		}
		uint16_t mark = clock.milliseconds;

		draw();

		while (inbox_first == inbox_last && delay > 0) { wait_ext(&mark, &delay); }

		if (delay > 0) {
			enum Action max_result = GAME_NONE;

			while (inbox_first != inbox_last) {
				const enum Action result = action();
				if (result > max_result) { max_result = result; }
			}

			if (max_result == GAME_NONE) { continue; }

			if (max_result == GAME_PAUSE) {
				game_paused = true;
				delay = DELAY_PAUSE;
				continue;
			}

			if (max_result == GAME_RESTART) {
				game_initialize();
				delay = DELAY_MAXIMUM;
				continue;
			}

			if (max_result == GAME_EXIT) { break; }
		}

		delay = DELAY_MAXIMUM;

		const enum Outcome result = move();

		if (result != NONE) {
			game_started = false;
			break;
		}
	}

	window_close(original_size);
}

enum Action action() {
	const char character = read_char();

	if (character == ANSI_ESCAPE) {
		if (read_char() == '[') { return action_movement(read_char()); }
		return GAME_NONE;
	}

	switch (character) {
		case 'w':
		case 'k': return action_movement(ABOVE);
		case 's':
		case 'j': return action_movement(BELOW);
		case 'd':
		case 'l': return action_movement(FORWARD);
		case 'a':
		case 'h': return action_movement(BACKWARD);
		case 'p': return GAME_PAUSE;
		case 'r': return GAME_RESTART;
		case 'x': return GAME_EXIT;
		default: return GAME_NONE;
	}
}

enum Action action_movement(const char character) {
	const enum Navigation navigation = (enum Navigation) character;
	enum Direction direction;

	switch (navigation) {
		case ABOVE: direction = UP; break;
		case BELOW: direction = DOWN; break;
		case FORWARD: direction = RIGHT; break;
		case BACKWARD: direction = LEFT; break;
		default: return GAME_NONE;
	}

	if (direction != snake_momentum && direction != (snake_momentum + 2) % 4) {
		if (!game_started) { game_started = true; }
		snake_direction = direction;
		return GAME_MOVEMENT;
	}

	return GAME_NONE;
}

enum Outcome move() {
	const struct Position head = snake_path[snake_head];
	const struct Position tail = snake_path[snake_tail];
	const struct Position delta = DIRECTIONS[snake_direction];
	const struct Position new_head = {head.x + delta.x, head.y + delta.y};
	const uint8_t new_square = game_board[new_head.y][new_head.x];

	if (new_head.x < 0 || new_head.y < 0 || new_head.x >= WIDTH || new_head.y >= HEIGHT) { return LOSS; }
	if (new_square >= SNAKE_N && new_square <= SNAKE_SE) { return LOSS; }

	const bool apple = new_square == APPLE;

	move_head(head, new_head);

	if (apple) { move_apple(); }
	else { move_tail(tail); }

	if ((snake_head + 1) % PATH == snake_tail % PATH) { return WIN; }

	return NONE;
}

void move_head(const struct Position head, const struct Position new_head) {
	game_board[head.y][head.x] = GAME_TABLE[snake_momentum][snake_direction];
	snake_head = (snake_head + 1) % PATH;
	snake_path[snake_head] = new_head;

	switch (snake_direction) {
		case UP: game_board[new_head.y][new_head.x] = SNAKE_N; break;
		case DOWN: game_board[new_head.y][new_head.x] = SNAKE_S; break;
		case LEFT: game_board[new_head.y][new_head.x] = SNAKE_W; break;
		case RIGHT: game_board[new_head.y][new_head.x] = SNAKE_E; break;
	}

	snake_momentum = snake_direction;
}

void move_tail(const struct Position tail) {
	game_board[tail.y][tail.x] = BLANK;
	snake_path[snake_tail] = POSITION_EMPTY;
	snake_tail = (snake_tail + 1) % PATH;
}

void move_apple() {
	struct Position apple;

	do { apple = (struct Position) {(int32_t) random_range(0, WIDTH - 1), (int32_t) random_range(0, HEIGHT - 1)}; }
	while (game_board[apple.y][apple.x] != BLANK);

	game_board[apple.y][apple.x] = APPLE;
}

void draw() {
	for (size_t x = 0; x < WIDTH + 2; x++) {
		for (size_t y = 0; y < HEIGHT + 2; y++) { draw_square(x, y); }
	}

	format_reset();
}

void draw_square(const size_t x, const size_t y) {
	cursor_position(x + 1, y + 1);

	if (x == 0 || y == 0 || x == WIDTH + 1 || y == HEIGHT + 1) {
		draw_square_wall(x, y);
		return;
	}

	draw_square_board(x, y);
	format_reset();
}

void draw_square_wall(const size_t x, const size_t y) {
	format_style(BOLD);

	if (x == 0 && y == 0) { print(SYMBOL_DOUBLE_NW); }
	else if (x == WIDTH + 1 && y == 0) { print(SYMBOL_DOUBLE_NE); }
	else if (x == 0 && y == HEIGHT + 1) { print(SYMBOL_DOUBLE_SW); }
	else if (x == WIDTH + 1 && y == HEIGHT + 1) { print(SYMBOL_DOUBLE_SE); }
	else if (x == 0 || x == WIDTH + 1) { print(SYMBOL_DOUBLE_V); }
	else if (y == 0 || y == HEIGHT + 1) { print(SYMBOL_DOUBLE_H); }
}

void draw_square_board(const size_t x, const size_t y) {
	const uint8_t tile = game_board[y - 1][x - 1];

	if (tile == BLANK) {
		format_style(FAINT);
		print(SYMBOL_DOT);
		return;
	}

	if (tile == APPLE) {
		format_style(COLOR_FOREGROUND + RED + COLOR_BRIGHT);
		format_style(BOLD);
		print(SYMBOL_CIRCLE);
		return;
	}

	format_style(COLOR_FOREGROUND + GREEN + COLOR_BRIGHT);
	format_style(BOLD);

	switch (tile) {
		case SNAKE_N: print(SYMBOL_ARROW_N); break;
		case SNAKE_S: print(SYMBOL_ARROW_S); break;
		case SNAKE_W: print(SYMBOL_ARROW_W); break;
		case SNAKE_E: print(SYMBOL_ARROW_E); break;
		case SNAKE_V: print(SYMBOL_SINGLE_V); break;
		case SNAKE_H: print(SYMBOL_SINGLE_H); break;
		case SNAKE_NW: print(SYMBOL_SINGLE_NW); break;
		case SNAKE_NE: print(SYMBOL_SINGLE_NE); break;
		case SNAKE_SW: print(SYMBOL_SINGLE_SW); break;
		case SNAKE_SE: print(SYMBOL_SINGLE_SE); break;
		default: print(" ");
	}
}

void window_open() {
	format_resize(WIDTH + 4, HEIGHT + 4);
	cursor_visibility(false);
}

void window_close(const struct Position original_size) {
	format_resize(original_size.x, original_size.y);
	cursor_visibility(true);
}
