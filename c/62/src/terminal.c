#include "terminal.h"
#include "library.h"
#include "literals.h"
#include "serial.h"

void terminal() {
	char buffer[MAX_INPUT];

	while (true) {
		memset(buffer, '\0', sizeof buffer);
		format_reset();

		print_reset("[");
		format(FAINT);
		print_reset(PROMPT_USER);
		print_reset(".");
		format(FAINT);
		print_reset(PROMPT_SYSTEM);
		print_reset("] > ");

		read_line(buffer);
		execute(buffer);
	}
}

void execute(const char * input) {
	char arguments[MAX_ARGUMENTS][MAX_ARGUMENT_LENGTH];
	memset(arguments, '\0', sizeof arguments);
	const size_t count = parse_arguments(arguments, input);

	if (count == 0) { return; }

	const uint8_t command = parse_command(arguments[0]);

	switch (command) {
		case EMPTY: break;
		case EXIT: exit(count > 1 ? parse_int(arguments[1]) : 0); break;
		case HELP:
			print(MESSAGE_PREFIX);
			print_line(MESSAGE_COMMAND_HELP);
			break;
		case CLEAR: format_clear(); break;
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
	int index = 0;
	int count = 0;
	int length = 0;
	bool in_quote = false;

	do {
		c = input[index];
		index++;

		if (is_empty(c) && !in_quote) {
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
		print_line(MESSAGE_ERROR_PARSE);
		return 0;
	}

	if (length > 0) {
		arguments[count][length] = '\0';
		count++;
	}

	return count;
}

uint8_t parse_command(const char * input) {
	if (strcmp(input, "") == 0) { return EMPTY; }
	if (strcmp(input, COMMAND_EXIT) == 0) { return EXIT; }
	if (strcmp(input, COMMAND_HELP) == 0) { return HELP; }
	if (strcmp(input, COMMAND_CLEAR) == 0) { return CLEAR; }
	return UNKNOWN;
}