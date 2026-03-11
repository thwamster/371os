#pragma once

#define RESERVED ""

#define PROMPT_USER "root"
#define PROMPT_SYSTEM "thwamsters-riscv64"

#define COMMAND_EXIT "exit"
#define COMMAND_HELP "help"
#define COMMAND_CLEAR "clear"

#define MESSAGE_PREFIX "isis: "
#define MESSAGE_ERROR_PARSE "unable to parse command"
#define MESSAGE_COMMAND_UNKNOWN "command not found"
#define MESSAGE_COMMAND_HELP \
	" \
	 \r\n exit : ends the program\
	 \r\n help : prints this menu again\
	 \r\n clear : cleans the terminal screen"

#define ERROR_INTERRUPTION "INTERRUPTION OCCURED: "
#define ERROR_EXCEPTION "EXCEPTION OCCURED: "

static const char * ERROR_INTERRUPTIONS[] = {
	RESERVED, "Supervisor software interrupt", RESERVED, RESERVED, RESERVED, "Supervisor timer interrupt", RESERVED, RESERVED,
	RESERVED, "Supervisor external interrupt", RESERVED, RESERVED, RESERVED, "Counter-overflow interrupt", RESERVED, RESERVED,
};

static const char * ERROR_EXCEPTIONS[] = {
	"Instruction address misaligned",
	"Instruction access fault",
	"Illegal instruction",
	"Breakpoint",
	"Load address misaligned",
	"Load access fault",
	"Store/AMO address misaligned",
	"Store/AMO access fault",
	"Environment call from U-mode",
	"Environment call from S-mode",
	RESERVED,
	RESERVED,
	"Instruction page fault",
	"Load page fault",
	RESERVED,
	"Store/AMO page fault",
	RESERVED,
	RESERVED,
	"Software check",
	"Hardware error",
	RESERVED,
	RESERVED,
	RESERVED,
	RESERVED,
};