#pragma once

#define RESERVED ""

#define PROMPT_USER "thwamster"
#define PROMPT_SYSTEM "qemu-system-riscv64"

#define COMMAND_EXIT "exit"
#define COMMAND_HELP "help"
#define COMMAND_CLEAR "clear"
#define COMMAND_TIME "time"
#define COMMAND_EXCEPTION "exception"
#define COMMAND_SNAKE "snake"

#define MESSAGE_PREFIX "isis: "
#define MESSAGE_TIME_PROMPT "Enter time (HHMMSS): "
#define MESSAGE_TIME_ERROR "unable to parse time"
#define MESSAGE_COMMAND_ERROR "unable to parse input"
#define MESSAGE_COMMAND_UNKNOWN "command not found"
#define MESSAGE_COMMAND_HELP \
	" \
	 \r\n 'exit' : ends the program\
	 \r\n 'clear' : cleans the terminal screen\
	 \r\n 'time' : displays the time\
	 \r\n 'snake' : launches a game of snake\
	 \r\n 'help' : prints this menu again "
#define MESSAGE_COMMAND_TIME "time: "

#define ERROR_INTERRUPTION "Interruption occured; "
#define ERROR_EXCEPTION "Exception occured; "

#define SYMBOL_DOT "·"
#define SYMBOL_CIRCLE "●"
#define SYMBOL_ARROW_N "▲"
#define SYMBOL_ARROW_W "◄"
#define SYMBOL_ARROW_S "▼"
#define SYMBOL_ARROW_E "►"
#define SYMBOL_SINGLE_V "┃"
#define SYMBOL_SINGLE_H "━"
#define SYMBOL_SINGLE_NW "┏"
#define SYMBOL_SINGLE_NE "┓"
#define SYMBOL_SINGLE_SW "┗"
#define SYMBOL_SINGLE_SE "┛"
#define SYMBOL_DOUBLE_V "║"
#define SYMBOL_DOUBLE_H "═"
#define SYMBOL_DOUBLE_NW "╔"
#define SYMBOL_DOUBLE_NE "╗"
#define SYMBOL_DOUBLE_SW "╚"
#define SYMBOL_DOUBLE_SE "╝"

#define TEST_INITIATE "Initiating testing. "
#define TEST_RUN "Running test #"
#define TEST_EXECUTE_EXPECTED "Expected execution. "
#define TEST_EXECUTE_UNEXPECTED "Unexpected execution. "
#define TEST_EXCEPTION_EXPECTED "Expected exception. "
#define TEST_EXCEPTION_UNEXPECTED "Expected exception. "
#define TEST_COMPLETE "All tests completed. Tests passed: "
#define TEST_PASS "Test Result: PASS."
#define TEST_FAIL "Test Result: FAIL."

extern const char * ERROR_INTERRUPTIONS[];
extern const char * ERROR_EXCEPTIONS[];
