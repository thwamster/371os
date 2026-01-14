#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 1024
#define MAX_LENGTH 1024

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

int main(int argc, char **argv) {
	char file_paths[MAX_FILES][MAX_LENGTH];
	int files = 0;
	int total[5] = {0, 0, 0, 0, 0};
	int bytes = 0;
	int chars = 0;
	int lines = 0;
	int max_line_length = 0;
	int words = 0;

	for (int i = 1; i < argc; i++) {
		const char *arg = argv[i];

		if (strcmp(arg, "--help") == 0) {
			printf(HELP_MENU);
			return 0;
		}
		if (strcmp(arg, "--version") == 0) {
			printf(VERSION_MENU);
			return 0;
		}
		if (strcmp(arg, "-c") == 0 || strcmp(arg, "--bytes") == 0) {
			bytes = 1;
		}
		else if (strcmp(arg, "-m") == 0 || strcmp(arg, "--chars") == 0) {
			chars = 1;
		}
		else if (strcmp(arg, "-l") == 0 || strcmp(arg, "--lines") == 0) {
			lines = 1;
		}
		else if (strcmp(arg, "-L") == 0 || strcmp(arg, "--max-line-length") == 0) {
			max_line_length = 1;
		}
		else if (strcmp(arg, "-w") == 0 || strcmp(arg, "--words") == 0) {
			words = 1;
		}
		else if (strncmp(arg, "--files0-from", 13) == 0) {
			// strcpy(file_paths[files], "bozo"); // fix
			// files++;
		}
		else {
			// strcpy(file_paths[files], arg);
			// files++;
		}
	}

	if (files == 0) {
		char buffer[MAX_LENGTH];
		fgets(buffer, MAX_LENGTH, stdin);
		printf("TODO: STDIN File Info\n");
	}

	for (int i = 0; i < files; i++) {
		char *path = file_paths[i];
		FILE *file = fopen(path, "r");
		if (file == NULL) {
			printf("my_wc: %s: No such file or directory\n", path);
			fclose(file);
			continue;
		}
		printf("TODO: PATH File Info\n");
	}

	if (files > 1) {
		printf("TODO: Total\n");
	}

	return 0;
}