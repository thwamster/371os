#include "graphics.h"
#include "game.h"
#include "literals.h"
#include "serial.h"

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
		format_style(game_status == OVER_LOSS ? FAINT : BOLD);
		print(SYMBOL_CIRCLE);
		return;
	}

	format_style(COLOR_FOREGROUND + GREEN + COLOR_BRIGHT);

	format_style(game_status == OVER_LOSS ? FAINT : BOLD);

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

void draw_position(const struct Position position) { draw_square(position.x + 1, position.y + 1); }