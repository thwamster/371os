#include <ctype.h>
#include <math.h>
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

#define TOO_LONG 0
#define NO_SUCH 1
#define EXTRA_OPERAND 2
#define UNRECOGNIZED_OPTION 3
#define INVALID_OPTION 4
#define CANNOT_OPEN 5

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
	"      --version  output version information and exit\n" \
	"\n" \
	"GNU coreutils online help: <https://www.gnu.org/software/coreutils/>\n" \
	"Report any translation bugs to <https://translationproject.org/team/>\n" \
	"Full documentation <https://www.gnu.org/software/coreutils/wc>\n" \
	"or available locally via: info '(coreutils) wc invocation'\n"

#define VERSION_MENU \
	"wc (GNU coreutils) 9.9\n" \
	"Copyright (C) 2025 Free Software Foundation, Inc.\n" \
	"License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\n" \
	"This is free software: you are free to change and redistribute it.\n" \
	"There is NO WARRANTY, to the extent permitted by law.\n" /* "\n" \
	"Written by Paul Rubin and David MacKenzie." */

char file_paths[MAX_FILES][MAX_LENGTH];
int file_data[MAX_FILES][5];
int file_count = 0;
int enable_standard = 1;
int enable_files0 = -1;

int main(int argc, char **argv);
void input();
void output();
int parse_file(const char *s);
int parse_files0(FILE *f);
int parse_argument(const char *s);
int parse_option_long(const char *s);
int parse_option_short(const char *s);
void enable_option(int n);
void analyze_file(FILE *f, int n);
int throw_error(const char *s, int n);