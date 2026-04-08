#include "terminal.h"
#include "library.h"
#include "literals.h"
#include "serial.h"
#include "system.h"
#include "timer.h"

void terminal() {
	char buffer[MAX_INPUT];

	str_clear(buffer, MAX_INPUT);
	format_reset();
	print(MESSAGE_TIME_PROMPT);
	read_line(buffer);

	while (!parse_time(buffer)) {
		format_reset();
		print(MESSAGE_PREFIX);
		print(MESSAGE_TIME_ERROR);
		print(" \'");
		print(buffer);
		print_line("\'");

		print(MESSAGE_TIME_PROMPT);

		str_clear(buffer, MAX_INPUT);
		read_line(buffer);
	}

	while (true) {
		str_clear(buffer, MAX_INPUT);
		format_reset();

		print_prompt();
		read_line(buffer);
		execute(buffer);
	}
}

void execute(const char * input) {
	char arguments[MAX_ARGUMENTS][MAX_ARGUMENT_LENGTH];
	memory_set(arguments, '\0', sizeof arguments);
	const size_t count = parse_arguments(arguments, input);

	if (count == 0) { return; }

	const uint8_t command = parse_command(arguments[0]);

	switch (command) {
		case EMPTY: break;
		case EXIT: exit(count > 1 ? str_to_num(arguments[1], 10) : 0); break;
		case HELP:
			print(MESSAGE_PREFIX);
			print_line(MESSAGE_COMMAND_HELP);
			break;
		case CLEAR: format_clear(); break;
		case TIME:
			print(MESSAGE_COMMAND_TIME);
			print_time();
			print_line("");
			break;
		case EXCEPTION: throw_exception(); break;
		default:
			print(MESSAGE_PREFIX);
			print("\'");
			print(arguments[0]);
			print("\' ");
			print_line(MESSAGE_COMMAND_UNKNOWN);
			break;
	}
}

size_t parse_arguments(char arguments[MAX_ARGUMENTS][MAX_ARGUMENT_LENGTH], const char * input) {
	char c;
	size_t index = 0;
	int8_t count = 0;
	int8_t length = 0;
	bool in_quote = false;

	do {
		c = input[index];
		index++;

		if (char_is_empty(c) && !in_quote) {
			if (length > 0) {
				arguments[count][length] = '\0';
				length = 0;
				count++;
			}

			continue;
		}

		if (c == '\"') {
			in_quote = !in_quote;
			continue;
		}

		arguments[count][length] = c;
		length++;
	}
	while (c != '\0');

	if (in_quote) {
		print(MESSAGE_PREFIX);
		print_line(MESSAGE_COMMAND_ERROR);
		print(" \'");
		print(input);
		print_line("\'");
		return 0;
	}

	if (length > 0) {
		arguments[count][length] = '\0';
		count++;
	}

	return count;
}

uint8_t parse_command(const char * input) {
	if (str_comp(input, "") == 0) { return EMPTY; }
	if (str_comp(input, COMMAND_EXIT) == 0) { return EXIT; }
	if (str_comp(input, COMMAND_HELP) == 0) { return HELP; }
	if (str_comp(input, COMMAND_CLEAR) == 0) { return CLEAR; }
	if (str_comp(input, COMMAND_TIME) == 0) { return TIME; }
	if (str_comp(input, COMMAND_EXCEPTION) == 0) { return EXCEPTION; }

	return UNKNOWN;
}

void print_prompt() {
	print_reset("[");
	format(FAINT);
	print_reset(PROMPT_USER);
	print_reset(".");
	format(FAINT);
	print_reset(PROMPT_SYSTEM);
	print_reset("] ~> ");
}