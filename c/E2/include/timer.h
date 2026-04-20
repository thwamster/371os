#pragma once

#include <stdbool.h>
#include <stdint.h>

struct Clock {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint16_t milliseconds;
};

static const uint8_t MAX_HOUR = 24;
static const uint8_t MAX_MINUTE = 60;
static const uint8_t MAX_SECOND = 60;
static const uint16_t MAX_MILLISECOND = 1000;

extern volatile struct Clock clock;

void schedule_time(uint64_t offset);
bool parse_time(const char * s);
bool set_time(uint8_t hours, uint8_t minutes, uint8_t seconds);
void update_time(void);
void print_time(void);
void wait(uint16_t delay);
void wait_ext(uint16_t * mark, uint16_t * delay);