#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 1024
#define MAX_LENGTH 1024

#define NEWLINES 0
#define WORDS 1
#define CHARS 2
#define BYTES 3
#define MAX_LINE_LENGTH 4

#define HELP_MENU \
	"Usage: wc [OPTION]... [FILE]...\n" \
	"  or:  wc [OPTION]... --files0-from=F\n" \
	"Print newline, word, and byte counts for each FILE, and a total line if\n" \
	"more than one FILE is specified.  A word is a non-zero-length sequence of\n" \
	"characters delimited by white space.\n" \
	"\n" \
	"With no FILE, or when FILE is -, read standard input.\n" \
	"\n" \
	"The options below may be used to select which counts are printed, always in\n" \
	"the following order: newline, word, character, byte, maximum line length.\n" \
	"  -c, --bytes            print the byte counts\n" \
	"  -m, --chars            print the character counts\n" \
	"  -l, --lines            print the newline counts\n" \
	"      --files0-from=F    read input from the files specified by\n" \
	"                           NUL-terminated names in file F;\n" \
	"                           If F is - then read names from standard input\n" \
	"  -L, --max-line-length  print the maximum display width\n" \
	"  -w, --words            print the word counts\n" \
	"      --help     display this help and exit\n" \
	"      --version  output version information and exit\n" /* "\n" \
	"GNU coreutils online help: <https://www.gnu.org/software/coreutils/>\n" \
	"Report any translation bugs to <https://translationproject.org/team/>\n" \
	"Full documentation <https://www.gnu.org/software/coreutils/wc>\n" \
	"or available locally via: info '(coreutils) wc invocation'" */

#define VERSION_MENU \
	"wc (GNU coreutils) 9.9\n" \
	"Copyright (C) 2025 Free Software Foundation, Inc.\n" \
	"License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n" \
	"This is free software: you are free to change and redistribute it.\n" \
	"There is NO WARRANTY, to the extent permitted by law.\n" /* "\n" \
	"Written by Paul Rubin and David MacKenzie." */

#define ERROR_TEXT "Try 'wc --help' for more information."

void get(FILE *path, int info[][5], int n);

int main(const int argc, char **argv) {
	char file_paths[MAX_FILES][MAX_LENGTH];
	int info[MAX_FILES][5];
	int standard = 1;
	int files = 0;

	info[0][CHARS] = -1;
	info[0][MAX_LINE_LENGTH] = -1;

	for (int i = 1; i < argc; i++) {
		const char *arg = argv[i];
		int value = -1;

		if (strcmp(arg, "--help") == 0) {
			printf(HELP_MENU);
			return 0;
		}
		if (strcmp(arg, "--version") == 0) {
			printf(VERSION_MENU);
			return 0;
		}
		if (strncmp(arg, "-", 1) == 0) {
			if (strcmp(arg, "-c") == 0 || strcmp(arg, "--bytes") == 0) {
				value = BYTES;
			}
			else if (strcmp(arg, "-m") == 0 || strcmp(arg, "--chars") == 0) {
				value = CHARS;
			}
			else if (strcmp(arg, "-l") == 0 || strcmp(arg, "--lines") == 0) {
				value = NEWLINES;
			}
			else if (strcmp(arg, "-L") == 0 || strcmp(arg, "--max-line-length") == 0) {
				value = MAX_LINE_LENGTH;
			}
			else if (strcmp(arg, "-w") == 0 || strcmp(arg, "--words") == 0) {
				value = WORDS;
			}
			else if (strncmp(arg, "--files0-from", 13) == 0) {
				// TODO: implement
				continue;
			}

			if (value == -1) {
				printf("my_wc: unrecognized option '%s'\nTry 'my_wc --help' for more information.\n", arg);
				return 0;
			}

			if (standard == 1) {
				info[0][NEWLINES] = -1;
				info[0][WORDS] = -1;
				info[0][BYTES] = -1;
				standard = 0;
			}

			info[0][value] = 0;
			continue;
		}

		files++;
		strcpy(file_paths[files], arg);
	}

	for (int i = 1; i <= files; i++) {
		const char *path = file_paths[i];
		FILE *file = fopen(path, "r");

		if (file == NULL) {
			info[i][0] = -1;
			continue;
		}

		get(file, info, i);

		fclose(file);
	}

	if (files == 0) {
		get(stdin, info, 1);
		files = 1;
	}

	// TODO: fix alignment
	for (int i = 1; i <= files; i++) {
		if (info[i][0] < 0) {
			printf("my_wc: %s: No such file or directory\n", file_paths[i]);
			continue;
		}

		for (int j = 0; j < 5; j++) {
			if (info[0][j] >= 0) {
				info[0][j] += info[i][j];
				printf(" %d", info[i][j]);
			}
		}

		printf(" %s\n", file_paths[i]);
	}

	if (files > 1) {
		for (int i = 0; i < 5; i++) {
			if (info[0][i] >= 0) {
				printf(" %d", info[0][i]);
			}
		}
		printf(" total\n");
	}

	return 0;
}

// TODO: fix output
void get(FILE *path, int info[][5], const int n) {
	char c;
	int line_length = 0;
	int word_length = 0;
	while ((c = (char) getc(path)) != EOF) {
		info[n][CHARS] += 1;
		info[n][BYTES] += 1;

		if (!isspace(c)) {
			line_length += 1;
			word_length += 1;
		}

		if (c == '\n') {
			if (info[n][MAX_LINE_LENGTH] < line_length) {
				info[n][MAX_LINE_LENGTH] = line_length;
			}

			info[n][NEWLINES] += 1;
			line_length = 0;
		}

		if (word_length > 0) {
			info[n][WORDS] += 1;
		}

		word_length = 0;
	}
}
