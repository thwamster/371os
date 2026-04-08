#include "timer.h"
#include "library.h"
#include "serial.h"

volatile Clock clock;

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
	return s && str_len(s) == 6 && set_time(str_to_num_len(s, 10, 2), str_to_num_len(s + 2, 10, 2), str_to_num_len(s + 4, 10, 2));
}

bool set_time(const uint8_t hours, const uint8_t minutes, const uint8_t seconds) {
	if (hours > 23 || minutes > 59 || seconds > 59) { return false; }

	clock.hours = hours;
	clock.minutes = minutes;
	clock.seconds = seconds;

	return true;
}

void update_time() {
	clock.seconds++;
	if (clock.seconds >= 60) {
		clock.seconds = 0;
		clock.minutes++;
	}
	if (clock.minutes >= 60) {
		clock.minutes = 0;
		clock.hours++;
	}
	if (clock.hours >= 24) { clock.hours = 0; }
}

void print_time() {
	print_num_ext(clock.hours, 10, false, 2);
	print_char(':');
	print_num_ext(clock.minutes, 10, false, 2);
	print_char(':');
	print_num_ext(clock.seconds, 10, false, 2);
}