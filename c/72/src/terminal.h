#pragma once

#include <stddef.h>
#include <stdint.h>

static const size_t MAX_ARGUMENTS = 0x10;
static const size_t MAX_ARGUMENT_LENGTH = 0x10;

enum Status { ERROR = -1, NORMAL = 0 };
enum Commands { UNKNOWN = -1, EMPTY = 0, EXIT = 1, HELP = 2, CLEAR = 3 };

void terminal();
void execute(const char * input);
size_t parse_arguments(char arguments[MAX_ARGUMENTS][MAX_ARGUMENT_LENGTH], const char * input);
uint8_t parse_command(const char * input);