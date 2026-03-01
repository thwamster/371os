#pragma once

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