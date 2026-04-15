#include "paging.h"
#include "allocator.h"
#include "layout.h"

struct PageTable * paging_initialize(void) {
	struct PageTable * root = (struct PageTable *) frame_allocate();

	map_section(root, &text_start, &text_end, FLAGS_CODE);
	map_section(root, &rodata_start, &rodata_end, FLAGS_PROTECTED);
	map_section(root, &data_start, &data_end, FLAGS_DATA);
	map_section(root, &bss_start, &bss_end, FLAGS_DATA);

	map_range(root, align_address(&bss_end), KERNEL_END, FLAGS_DATA);
	map_range(root, (uint64_t) UART, (uint64_t) VIRTIO_END, FLAGS_DATA);
	map_range(root, (uint64_t) PLIC_PRIORITY, (uint64_t) PLIC_CLAIM, FLAGS_DATA);
	map_range(root, (uint64_t) CLINT_BASE, (uint64_t) MTIME, FLAGS_DATA);
	map_range(root, (uint64_t) SIFIVE_EXIT, (uint64_t) SIFIVE_EXIT, FLAGS_DATA);

	uint64_t satp = SATP_SV39 | pa_to_ppn((uint64_t) root);
	asm volatile("csrw satp, %0" : : "r"(satp));
	asm volatile("sfence.vma");

	return root;
}

void map_page(struct PageTable * root, const uint64_t address_virt, const uint64_t address_phys, const uint64_t flags) {
	const uint64_t v2 = address_virt >> VPN1 & VPN_MASK;
	const uint64_t v1 = address_virt >> VPN2 & VPN_MASK;
	const uint64_t v0 = address_virt >> VPN3 & VPN_MASK;

	if (!(root->entries[v2] & VALID)) { root->entries[v2] = create_branch(frame_allocate()); }

	struct PageTable * level1 = (struct PageTable *) ppn_to_pa(root->entries[v2] >> PTE_PPN_SHIFT);
	if (!(level1->entries[v1] & VALID)) { level1->entries[v1] = create_branch(frame_allocate()); }

	struct PageTable * level0 = (struct PageTable *) ppn_to_pa(level1->entries[v1] >> PTE_PPN_SHIFT);
	level0->entries[v0] = create_leaf(address_phys, flags);
}

void map_section(struct PageTable * root, const char * start, const char * end, const uint64_t flags) {
	for (uint64_t a = (uint64_t) start; a < (uint64_t) end; a += FRAME_SIZE) { map_page(root, a, a, flags); }
}

void map_range(struct PageTable * root, const uint64_t start, const uint64_t end, const uint64_t flags) {
	for (uint64_t a = start; a <= end; a += FRAME_SIZE) { map_page(root, a, a, flags); }
}

uint64_t align_address(const volatile void * const address_phys) { return (uint64_t) address_phys & ~(FRAME_SIZE - 1); }
uint64_t pa_to_ppn(const uint64_t address_phys) { return address_phys >> PAGE_SHIFT; }
uint64_t ppn_to_pa(const uint64_t ppn) { return ppn << PAGE_SHIFT; }
uint64_t create_branch(const uint64_t address_phys_child) { return pa_to_ppn(address_phys_child) << PTE_PPN_SHIFT | VALID; }
uint64_t create_leaf(const uint64_t address_phys, const uint64_t flags) { return pa_to_ppn(address_phys) << PTE_PPN_SHIFT | flags | VALID | ACCESSED | DIRTY; }