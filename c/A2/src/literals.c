#include "literals.h"

const char * ERROR_INTERRUPTIONS[] = {
	RESERVED, "Supervisor software interrupt", RESERVED, RESERVED, RESERVED, "Supervisor timer interrupt", RESERVED, RESERVED,
	RESERVED, "Supervisor external interrupt", RESERVED, RESERVED, RESERVED, "Counter-overflow interrupt", RESERVED, RESERVED,
};

const char * ERROR_EXCEPTIONS[] = {
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
	RESERVED, RESERVED,
	"Instruction page fault",
	"Load page fault",
	RESERVED,
	"Store/AMO page fault",
	RESERVED, RESERVED,
	"Software check",
	"Hardware error",
	RESERVED, RESERVED, RESERVED, RESERVED,
};