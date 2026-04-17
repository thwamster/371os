#include "snake.h"
#include "allocator.h"
#include "random.h"
#include "serial.h"

#include <string.h>
#include <timer.h>

uint8_t game_board[HEIGHT][WIDTH];
struct Position snake_path[MAX_SNAKE_LENGTH];
enum Direction snake_direction;
enum Direction snake_momentum;
size_t snake_head = 0;
size_t snake_tail = 0;
bool game_on = false;

void snake_run() {
	const struct Position original_size = query_size();
	snake_window_open();
	snake_initialize();
	draw_board();

	while (true) {
		wait(250);
		snake_get_input();
		while (!game_on) {
			snake_get_input();
			draw_board();
		}
		draw_board();

		const enum Result result = snake_move();

		if (result != NONE) {
			wait(1000);
			game_on = false;
			break;
		}
	}

	snake_window_close(original_size);
}

void snake_get_input() {
	char buffer[MAX_INPUT];
	string_clear(buffer, MAX_INPUT);
	read_inbox(buffer, MAX_INPUT);

	for (int i = 0; buffer[i] != '\0'; i++) {
		char c = buffer[i];

		if (c == ANSI_ESCAPE) {
			i += 1;
			c = buffer[i];

			if (c == '[') {
				i += 1;
				c = buffer[i];

				const enum Navigation navigation = (enum Navigation) c;
				enum Direction direction;

				switch (navigation) {
					case ABOVE: direction = UP; break;
					case BELOW: direction = DOWN; break;
					case FORWARD: direction = RIGHT; break;
					case BACKWARD: direction = LEFT; break;
					default: continue;
				}

				if (direction != (snake_momentum + 2) % 4) {
					if (!game_on) { game_on = true; }

					snake_direction = direction;
				}
			}
			continue;
		}

		if (c == 'r') {
			snake_initialize();
			continue;
		}

		if (c == 'x') { break; }
	}
}

void snake_window_open() {
	format_resize(WIDTH + 4, HEIGHT + 4);
	cursor_visibility(false);
}

void snake_window_close(const struct Position original_size) {
	format_resize(original_size.x, original_size.y);
	cursor_visibility(true);
}

void snake_initialize() {
	memory_set(game_board, 0, HEIGHT * WIDTH);
	memory_set(snake_path, 0, MAX_SNAKE_LENGTH);
	snake_direction = RIGHT;
	snake_momentum = RIGHT;

	snake_head = sizeof(snake_set) / sizeof(struct Position) - 1;
	snake_tail = 0;

	for (size_t i = snake_tail; i <= snake_head; i++) {
		snake_path[i] = snake_set[i];
		game_board[snake_path[i].y][snake_path[i].x] = SNAKE;
	}

	game_board[apple_set.y][apple_set.x] = APPLE;

	game_on = false;
}

enum Result snake_move() {
	const struct Position head = snake_path[snake_head];
	const struct Position tail = snake_path[snake_tail];
	const struct Position delta = directions[snake_direction];
	const struct Position new_head = {head.x + delta.x, head.y + delta.y};

	if (new_head.x >= WIDTH || new_head.y >= HEIGHT) { return LOSS; }
	if (game_board[new_head.y][new_head.x] == SNAKE) { return LOSS; }

	const bool apple = game_board[new_head.y][new_head.x] == APPLE;

	snake_head = (snake_head + 1) % MAX_SNAKE_LENGTH;
	snake_path[snake_head] = new_head;
	game_board[new_head.y][new_head.x] = SNAKE;
	snake_momentum = snake_direction;

	if (apple) { apple_new(); }
	else {
		game_board[tail.y][tail.x] = BLANK;
		snake_path[snake_tail] = empty_position;
		snake_tail = (snake_tail + 1) % MAX_SNAKE_LENGTH;
	}

	if ((snake_head + 1) % MAX_SNAKE_LENGTH == snake_tail % MAX_SNAKE_LENGTH) { return WIN; }

	return NONE;
}

void apple_new() {
	struct Position apple;

	do { apple = (struct Position) {(int32_t) random_range(0, WIDTH), (int32_t) random_range(0, HEIGHT)}; }
	while (game_board[apple.y][apple.x] != BLANK);

	game_board[apple.y][apple.x] = APPLE;
}

void draw_board() {
	for (size_t x = 0; x < WIDTH + 2; x++) {
		for (size_t y = 0; y < HEIGHT + 2; y++) {
			cursor_position(x + 1, y + 1);

			if (x == 0 || y == 0 || x == WIDTH + 1 || y == HEIGHT + 1) {
				print_char('#');
				continue;
			}

			switch (game_board[y - 1][x - 1]) {
				case BLANK:
					format_style(FAINT);
					print_char('.');
					break;
				case SNAKE:
					format_style(COLOR_FOREGROUND + GREEN);
					print_char('S');
					break;
				case APPLE:
					format_style(COLOR_FOREGROUND + RED);
					print_char('A');
					break;
				default: print_char('?');
			}

			format_reset();
		}
	}
}