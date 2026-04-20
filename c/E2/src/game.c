#include "game.h"
#include "allocator.h"
#include "graphics.h"
#include "literals.h"
#include "random.h"
#include "serial.h"

#include <timer.h>

enum Status game_status = AWAITING;
volatile uint16_t game_delay = 0;
struct Position snake_path[PATH];
enum Direction snake_direction;
enum Direction snake_momentum;
size_t snake_head = 0;
size_t snake_tail = 0;
uint8_t game_board[HEIGHT][WIDTH];

void game_initialize() {
	memory_set(game_board, 0, HEIGHT * WIDTH);
	memory_set(snake_path, 0, PATH);

	snake_direction = RIGHT;
	snake_momentum = RIGHT;
	snake_head = sizeof(SET_SNAKE) / POSITION_SIZE - 1;
	snake_tail = 0;
	game_status = AWAITING;
	game_delay = DELAY_MAXIMUM;

	game_initialize_board();
	draw();
}

void game_initialize_board() {
	for (size_t i = snake_tail; i <= snake_head; i++) {
		snake_path[i] = SET_SNAKE[i];
		game_board[SET_SNAKE[i].y][SET_SNAKE[i].x] = SNAKE_H;
	}

	game_board[SET_SNAKE[snake_head].y][SET_SNAKE[snake_head].x] = SNAKE_E;

	for (size_t i = 0; i < sizeof(SET_APPLE) / POSITION_SIZE; i++) { game_board[SET_APPLE[i].y][SET_APPLE[i].x] = APPLE; }
}

void game_start() {
	format_clear();
	cursor_visibility(false);
	game_initialize();

	game_delay = DELAY_MAXIMUM;

	while (game_run()) {}

	format_clear();
}

bool game_run() {
	if (game_status == OVER_EXITING) { return false; }

	if (game_status == AWAITING) {
		action();
		return true;
	}

	if (game_status == PAUSED) {
		action();
		return true;
	}

	if (game_status == OVER_WIN || game_status == OVER_LOSS) {
		action();
		return true;
	}

	uint16_t mark = clock.milliseconds;

	while (inbox_first == inbox_last && game_delay > 0) { wait_ext(&mark, (uint16_t *) &game_delay); }

	if (game_delay > 0) {
		if (game_read()) { return true; }
	}

	game_delay = DELAY_MAXIMUM;
	game_status = move();

	if (game_status == OVER_WIN || game_status == OVER_LOSS) {
		draw();
		return true;
	}

	return true;
}

bool game_read() {
	enum Action max_result = GAME_NONE;

	while (inbox_first != inbox_last) {
		const enum Action result = action();
		if (result > max_result) { max_result = result; }
	}

	return max_result != GAME_MOVEMENT;
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
		case 'p': action_pause(); return GAME_PAUSE;
		case 'r': action_restart(); return GAME_RESTART;
		case 'x': action_exit(); return GAME_EXIT;
		default: return GAME_NONE;
	}
}

void action_pause() {
	if (game_status == RUNNING) { game_status = PAUSED; }
	else if (game_status == PAUSED) {
		game_status = RUNNING;
		game_delay = DELAY_PAUSE;
	}
}

void action_restart() {
	game_initialize();
	game_status = AWAITING;
	game_delay = DELAY_MAXIMUM;
}

void action_exit() { game_status = OVER_EXITING; }

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

	if (direction != (snake_momentum + 2) % 4) {
		if (game_status == AWAITING) { game_status = RUNNING; }

		if (direction != snake_momentum) {
			snake_direction = direction;
			return GAME_MOVEMENT;
		}
	}

	return GAME_NONE;
}

enum Status move() {
	const struct Position head = snake_path[snake_head];
	const struct Position tail = snake_path[snake_tail];
	const struct Position delta = DIRECTIONS[snake_direction];
	const struct Position new_head = {head.x + delta.x, head.y + delta.y};
	const uint8_t new_square = game_board[new_head.y][new_head.x];

	if (new_head.x < 0 || new_head.y < 0 || new_head.x >= WIDTH || new_head.y >= HEIGHT) { return OVER_LOSS; }
	if (new_square >= SNAKE_N && new_square <= SNAKE_SE) { return OVER_LOSS; }

	const bool apple = new_square == APPLE;

	move_head(head, new_head);

	if (apple) { move_apple(); }
	else { move_tail(tail); }

	if ((snake_head + 1) % PATH == snake_tail % PATH) { return OVER_WIN; }

	return RUNNING;
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
	draw_position(new_head);
	draw_position(head);
}

void move_tail(const struct Position tail) {
	game_board[tail.y][tail.x] = BLANK;
	snake_path[snake_tail] = POSITION_EMPTY;
	snake_tail = (snake_tail + 1) % PATH;
	draw_position(tail);
}

void move_apple() {
	struct Position apple;

	do { apple = (struct Position) {(int32_t) random_range(0, WIDTH - 1), (int32_t) random_range(0, HEIGHT - 1)}; }
	while (game_board[apple.y][apple.x] != BLANK);

	game_board[apple.y][apple.x] = APPLE;
	draw_position(apple);
}