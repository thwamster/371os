#include "system.h"
#include "allocator.h"
#include "exception.h"
#include "paging.h"

void initialize_system() {
	initialize_frame();
	struct PageTable * root = initialize_paging();
	enable_paging(root);
	initialize_exception();
}
