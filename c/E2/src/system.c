#include "system.h"
#include "allocator.h"
#include "paging.h"
#include "exception.h"

void initialize_system() {
	frame_initialize();
	paging_initialize();
	exception_initialize();
}
