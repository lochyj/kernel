#include "system/memory/paging.h"

extern uintptr_t placement_pointer;

extern void cr3_load_paging_directory(uintptr_t);

void* virtual_to_physical(void* virtual_address) {
    uint64_t pdindex = (uint64_t)virtual_address >> 22;
    uint64_t ptindex = (uint64_t)virtual_address >> 12 & 0x03FF;
 
    uint64_t *pd = (uint64_t *)0xFFFFF000;
    // Here you need to check whether the PD entry is present.

    uint64_t *pt = ((uint64_t *)0xFFC00000) + (0x400 * pdindex);
    // Here you need to check whether the PT entry is present.
 
    return (void *)((pt[ptindex] & ~0xFFF) + ((uint64_t)virtual_address & 0xFFF));
}

uintptr_t fill_page_table(uintptr_t memory_address) {
	//
}

/*
 * @param page_directory A pointer to the page directory to initialize.
 * Setup our paging directory, load it into the cr3 register, copy the kernel heap into the higher half, and enable paging.
 */
void install_paging(/*uint32_t memory_size*/) {

	uint32_t page_directory[1024] __attribute__((aligned(4096)));
	int i;
	for(i = 0; i < 1024; i++) {
		// This sets the following flags to the pages:
		//   Supervisor: Only kernel-mode can access them
		//   Write Enabled: It can be both read from and written to
		//   Not Present: The page table is not present
		page_directory[i] = 0x00000002;
	}

	uint32_t first_page_table[1024] __attribute__((aligned(4096)));

	// holds the physical address where we want to start mapping these pages to.
	// in this case, we want to map these pages to the very beginning of memory.
	unsigned int j;

	//we will fill all 1024 entries in the table, mapping 4 megabytes
	for(j = 0; j < 1024; j++) {
		// As the address is page aligned, it will always leave 12 bits zeroed.
		// Those bits are used by the attributes ;)
		first_page_table[j] = (j * 0x1000) | 3; // attributes: supervisor level, read/write, present.
	}

	// attributes: supervisor level, read/write, present
	page_directory[0] = ((unsigned int)first_page_table) | 3;

	register_interrupt_handler(14, page_fault_handler);

	load_paging_directory_into_cr3(page_directory);

}


/*
 * @param page_directory A pointer the the page directory to load into the CR3 register.
 * Loads a page directory into the CR3 register. This is simply a wrapper for the assembly function in paging.asm.
 */
void load_paging_directory_into_cr3(page_directory_t *page_directory) {
	cr3_load_paging_directory(page_directory->address_physical);
}

void page_fault_handler(registers_t* regs) {
	uint32_t faulting_address;
	// TODO: is the cr2 register going to be correct after a page fault??
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

	bool present = !(regs->err_code & 0x1); // Page not present
	bool rw = regs->err_code & 0x2;         // Write operation?
	bool us = regs->err_code & 0x4;         // Processor was in user-mode?
	bool reserved = regs->err_code & 0x8;   // Overwritten CPU-reserved bits of page entry?
	bool id = regs->err_code & 0x10;        // Caused by an instruction fetch?

	// Output an error message.
	printf("Page fault! ( ");
	if (present) {printf("present ");}
	if (rw) {printf("read-only ");}
	if (us) {printf("user-mode ");}
	if (reserved) {printf("reserved ");}
	printf(") at 0x%x\n", faulting_address);
	PANIC("Page fault");
}
