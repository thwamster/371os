#include "main.h"

int main(const int argc, char **argv) {
	file_data[0][CHARS] = -1;
	file_data[0][MAX_LINE_LENGTH] = -1;

	for (int i = 1; i < argc; i++) {
		const char *arg = argv[i];

		if (parse_argument(arg) == 1) {
			return 0;
		}
	}
	input();
	output();

	return 0;
}

void input() {
	for (int i = 1; i <= file_count; i++) {
		const char *path = file_paths[i];

		if (strcmp(path, "-") == 0) {
			analyze_file(stdin, i);
			continue;
		}

		FILE *file = fopen(file_paths[i], "r");

		if (file == NULL) {
			file_data[i][0] = -1;
			continue;
		}

		analyze_file(file, i);
		fclose(file);
	}

	if (file_count == 0) {
		analyze_file(stdin, 1);
		file_count = 1;
	}
}

void output() {
	for (int i = 1; i <= file_count; i++) {
		const char *path = file_paths[i];

		if (strlen(path) > MAX_LENGTH) {
			throw_error(path, TOO_LONG);
			continue;
		}

		if (file_data[i][0] < 0) {
			throw_error(path, NO_SUCH);
			continue;
		}

		for (int j = 0; j < 5; j++) {
			if (file_data[0][j] >= 0) {
				printf(" %*d", file_data[file_count + 1][j], file_data[i][j]);
			}
		}

		printf(" %s\n", path);
	}

	if (file_count > 1) {
		for (int j = 0; j < 5; j++) {
			if (file_data[0][j] >= 0) {
				printf(" %*d", file_data[file_count + 1][j], file_data[0][j]);
			}
		}
		printf(" total\n");
	}
}

int parse_argument(const char *s) {
	if (strcmp(s, "-") != 0) {
		if (strncmp(s, "--", 2) == 0) {
			return parse_option_long(s + 2);
		}
		if (strncmp(s, "-", 1) == 0) {
			return parse_option_short(s + 1);
		}
	}

	return parse_file(s);
}

int parse_file(const char *s) {
	if (enable_files0 == 1) {
		return throw_error(s, EXTRA_OPERAND);
	}

	enable_files0 = 0;
	file_count += 1;
	strcpy(file_paths[file_count], s);
	return 0;
}

int parse_files0(FILE *f) {
	char c;
	int len = 0;
	file_count = 1;

	while ((c = (char) getc(f)) != EOF) {
		if (c == '\0') {
			file_count += 1;
			len = 0;

			FILE *now = fopen(file_paths[file_count], "r");
			if (now == NULL) {
				return throw_error(file_paths[file_count], CANNOT_OPEN);
			}
			fclose(now);
		}

		file_paths[file_count][len] = c;
		file_paths[file_count][len + 1] = '\0';
		len++;
	}

	FILE *now = fopen(file_paths[file_count], "r");
	if (now == NULL) {
		return throw_error(file_paths[file_count], CANNOT_OPEN);
	}
	fclose(now);

	return 0;
}

int parse_option_long(const char *s) {
	if (strcmp(s, "help") == 0) {
		printf(HELP_MENU);
		return 1;
	}
	if (strcmp(s, "version") == 0) {
		printf(VERSION_MENU);
		return 1;
	}
	if (strncmp(s, "files0-from=", 12) == 0) {
		if (enable_files0 == 0) {
			return throw_error(s, EXTRA_OPERAND);
		}

		enable_files0 = 1;

		FILE *file = (strcmp(s, "-") == 0) ? file = stdin : fopen(s + 12, "r");
		if (file == NULL) {
			return throw_error(s, NO_SUCH);
		}
		return parse_files0(file);
	}
	if (strcmp(s, "bytes") == 0) {
		enable_option(BYTES);
	}
	else if (strcmp(s, "chars") == 0) {
		enable_option(CHARS);
	}
	else if (strcmp(s, "lines") == 0) {
		enable_option(NEWLINES);
	}
	else if (strcmp(s, "max-line-length") == 0) {
		enable_option(MAX_LINE_LENGTH);
	}
	else if (strcmp(s, "words") == 0) {
		enable_option(WORDS);
	}
	else {
		return throw_error(s, UNRECOGNIZED_OPTION);
	}

	return 0;
}

int parse_option_short(const char *s) {
	while (strlen(s) > 0) {
		if (strncmp(s, "c", 1) == 0) {
			enable_option(BYTES);
		}
		else if (strncmp(s, "m", 1) == 0) {
			enable_option(CHARS);
		}
		else if (strncmp(s, "l", 1) == 0) {
			enable_option(NEWLINES);
		}
		else if (strncmp(s, "L", 1) == 0) {
			enable_option(MAX_LINE_LENGTH);
		}
		else if (strncmp(s, "w", 1) == 0) {
			enable_option(WORDS);
		}
		else {
			return throw_error(s, INVALID_OPTION);
		}

		s = s + 1;
	}

	return 0;
}

void enable_option(const int n) {
	if (enable_standard == 1) {
		file_data[0][NEWLINES] = -1;
		file_data[0][WORDS] = -1;
		file_data[0][BYTES] = -1;
		enable_standard = 0;
	}

	file_data[0][n] = 0;
}

void analyze_file(FILE *f, const int n) {
	char c;
	int line_length = 0;
	int word_length = 0;

	while ((c = (char) getc(f)) != EOF) {
		file_data[n][CHARS] += 1;
		file_data[n][BYTES] += 1;

		if (c == '\t') {
			line_length += 8 - (line_length % 8);
		}
		else if (c == '\n') {
			line_length = line_length > 0 ? line_length : 0;
			if (file_data[n][MAX_LINE_LENGTH] < line_length) {
				file_data[n][MAX_LINE_LENGTH] = line_length;
			}
			file_data[n][NEWLINES] += 1;
			line_length = 0;
		}
		else {
			line_length += 1;
		}

		if (!isspace(c)) {
			word_length += 1;
		}
		else {
			if (word_length > 0) {
				file_data[n][WORDS] += 1;
				word_length = 0;
			}
		}
	}

	if (word_length > 0) {
		file_data[n][WORDS] += 1;
	}

	for (int i = 0; i < 4; i++) {
		if (file_data[0][i] >= 0) {
			file_data[0][i] += file_data[n][i];
			if (file_data[0][i] > 0) {
				file_data[file_count + 1][i] = (int) floor(log10(abs(file_data[0][i]))) + 1;
			}
		}
	}

	line_length = line_length > 0 ? line_length : 0;
	if (file_data[n][MAX_LINE_LENGTH] < line_length) {
		file_data[n][MAX_LINE_LENGTH] = line_length;
	}

	if (file_data[0][MAX_LINE_LENGTH] >= 0 && file_data[0][MAX_LINE_LENGTH] < file_data[n][MAX_LINE_LENGTH]) {
		file_data[0][MAX_LINE_LENGTH] = file_data[n][MAX_LINE_LENGTH];
	}

	if (file_data[0][MAX_LINE_LENGTH] > 0) {
		file_data[file_count + 1][MAX_LINE_LENGTH] = (int) floor(log10(abs(file_data[0][MAX_LINE_LENGTH]))) + 1;
	}
}

int throw_error(const char *s, const int n) {
	switch (n) {
		case TOO_LONG: printf("my_wc: %s: File name too long\n", s); break;
		case NO_SUCH: printf("my_wc: %s: No such file or directory\n", s); break;
		case EXTRA_OPERAND:
			{
				printf("wc: extra operand '%s'\nfile operands cannot be combined with --files0-from\nTry 'wc --help' for more information.\n", s);
				break;
			}
		case UNRECOGNIZED_OPTION: printf("my_wc: unrecognized option '--%s'\nTry 'my_wc --help' for more information.\n", s); break;
		case INVALID_OPTION: printf("my_wc: invalid option -- '%c'\nTry 'my_wc --help' for more information.\n", *s); break;
		case CANNOT_OPEN:
			{
				printf("wc: cannot open '");
				for (int i = 0; i < strlen(s); i++) {
					if (isprint(s[i])) {
						printf("%c", s[i]);
					}
					else {
						printf("'\\x%02x'", s[i]);
					}
				}
				printf("' for reading: No such file or directory");
				break;
			}
		default:;
	}
	return 1;
}
