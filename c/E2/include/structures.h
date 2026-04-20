#pragma once

struct Position {
	int32_t x;
	int32_t y;
};

static const struct Position POSITION_EMPTY = {0, 0};
static const size_t POSITION_SIZE = sizeof(struct Position);