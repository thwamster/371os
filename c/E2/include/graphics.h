#pragma once

#include <stddef.h>
#include <stdint.h>

#include "structures.h"

void draw(void);
void draw_square(size_t x, size_t y);
void draw_square_wall(size_t x, size_t y);
void draw_square_board(size_t x, size_t y);
void draw_position(struct Position position);