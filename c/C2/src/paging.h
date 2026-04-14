#pragma once

#include <stddef.h>
#include <stdint.h>

enum PTEFlags {
	VALID = 1 << 0,
	READ = 1 << 1,
	WRITE = 1 << 2,
	EXECUTE = 1 << 3,
	USER = 1 << 4,
	GLOBAL = 1 << 5,
	ACCESSED = 1 << 6,
	DIRTY = 1 << 7,
};

enum VPNMasks {
	VPN1 = 30,
	VPN2 = 21,
	VPN3 = 12,
};

static const uint64_t PAGE_SHIFT = 12;
static const uint64_t PTE_PPN_SHIFT = 10;
static const uint64_t VPN_MASK = 0x1FF;
static const uint64_t SATP_SV39 = 8ULL << 60;

static const uint64_t FLAGS_PROTECTED = READ | ACCESSED | DIRTY;
static const uint64_t FLAGS_CODE = READ | EXECUTE | ACCESSED | DIRTY;
static const uint64_t FLAGS_DATA = READ | WRITE | ACCESSED | DIRTY;

extern char stack_top;
extern char text_start;
extern char text_end;
extern char rodata_start;
extern char rodata_end;
extern char data_start;
extern char data_end;
extern char bss_start;
extern char bss_end;

struct __attribute__((aligned(4096))) PageTable {
	uint64_t entries[512];
};

struct PageTable * initialize_paging(void);
void enable_paging(struct PageTable * root);
void map_page(struct PageTable * root, uint64_t address_virt, uint64_t address_phys, uint64_t flags);
void map_range(struct PageTable * root, uint64_t start, uint64_t end, uint64_t flags);
void map_section(struct PageTable * root, const char * start, const char * end, uint64_t flags);
uint64_t align_address(const volatile void * address_phys);
uint64_t pa_to_ppn(uint64_t address_phys);
uint64_t ppn_to_pa(uint64_t ppn);
uint64_t create_branch(uint64_t address_phys_child);
uint64_t create_leaf(uint64_t address_phys, uint64_t flags);