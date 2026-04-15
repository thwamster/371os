#include "system.h"
#include "allocator.h"
#include "exception.h"
#include "paging.h"

void initialize_system() {
	frame_initialize();
	paging_initialize();
	exception_initialize();
}
