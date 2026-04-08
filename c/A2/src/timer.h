#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} Clock;

extern volatile Clock clock;

void schedule_time(uint64_t offset);
bool parse_time(const char * s);
bool set_time(uint8_t hours, uint8_t minutes, uint8_t seconds);
void update_time(void);
void print_time(void);