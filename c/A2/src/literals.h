#pragma once

#define RESERVED ""

#define PROMPT_USER "thwamster"
#define PROMPT_SYSTEM "qemu-system-riscv64"

#define COMMAND_EXIT "exit"
#define COMMAND_HELP "help"
#define COMMAND_CLEAR "clear"
#define COMMAND_TIME "time"
#define COMMAND_EXCEPTION "exception"

#define MESSAGE_PREFIX "isis: "
#define MESSAGE_TIME_PROMPT "Enter time (HHMMSS): "
#define MESSAGE_TIME_ERROR "unable to parse time"
#define MESSAGE_COMMAND_ERROR "unable to parse input"
#define MESSAGE_COMMAND_UNKNOWN "command not found"
#define MESSAGE_COMMAND_HELP \
	" \
	 \r\n exit : ends the program\
	 \r\n clear : cleans the terminal screen\
	 \r\n time : displays the time\
	 \r\n help : prints this menu again "
#define MESSAGE_COMMAND_TIME "time: "

#define ERROR_INTERRUPTION "Interruption occured; "
#define ERROR_EXCEPTION "Exception occured; "

#define TEST_INITIATE "Initiating testing. "
#define TEST_RUN "Running test "
#define TEST_EXECUTE_EXPECTED "Expected execution. "
#define TEST_EXECUTE_UNEXPECTED "Unexpected execution. "
#define TEST_EXCEPTION_EXPECTED "Expected exception. "
#define TEST_EXCEPTION_UNEXPECTED "Expected exception. "
#define TEST_COMPLETE "All tests completed. Tests passed: "

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
