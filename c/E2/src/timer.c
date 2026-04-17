#include "timer.h"
#include "serial.h"
#include "string.h"

volatile struct Clock clock;

void schedule_time(const uint64_t offset) {
	uint64_t time;
	__asm__ volatile("csrr %0, time" : "=r"(time));
	__asm__ volatile("li a7, 0x54494D45 \n"
					 "li a6, 0 \n"
					 "mv a0, %0 \n"
					 "ecall \n"
					 :
					 : "r"(time + offset)
					 : "a0", "a1", "a6", "a7", "memory");
}

bool parse_time(const char * s) {
	return s && string_length(s) == 6 && set_time(string_to_num_ext(s, 10, 2), string_to_num_ext(s + 2, 10, 2), string_to_num_ext(s + 4, 10, 2));
}

bool set_time(const uint8_t hours, const uint8_t minutes, const uint8_t seconds) {
	if (hours >= MAX_HOUR || minutes >= MAX_MINUTE || seconds >= MAX_SECOND) { return false; }

	clock.hours = hours;
	clock.minutes = minutes;
	clock.seconds = seconds;

	return true;
}

void update_time() {
	clock.milliseconds += 10;

	if (clock.milliseconds >= MAX_MILLISECOND) {
		clock.milliseconds -= MAX_MILLISECOND;
		clock.seconds++;
	}
	if (clock.seconds >= MAX_SECOND) {
		clock.seconds = 0;
		clock.minutes++;
	}
	if (clock.minutes >= MAX_MINUTE) {
		clock.minutes = 0;
		clock.hours++;
	}

	if (clock.hours >= MAX_HOUR) { clock.hours = 0; }
}

void print_time() {
	print_num_ext(clock.hours, 10, false, 2);
	print_char(':');
	print_num_ext(clock.minutes, 10, false, 2);
	print_char(':');
	print_num_ext(clock.seconds, 10, false, 2);
}

void wait(uint64_t milliseconds) {
	uint16_t prev = clock.milliseconds;

	while (milliseconds > 0) {
		__asm__ volatile("wfi");

		uint16_t now = clock.milliseconds;

		if (now != prev) {
			uint16_t delta = now >= prev ? now - prev : MAX_MILLISECOND - prev + now;
			milliseconds = milliseconds > delta ? milliseconds - delta : 0;
			prev = now;
		}
	}
}