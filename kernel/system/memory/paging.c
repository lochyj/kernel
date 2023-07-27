#include "system/memory/paging.h"

extern uintptr_t placement_pointer;

extern void cr3_load_paging_directory(uintptr_t);
extern void cr0_enable_paging();

/*
 * @param page_directory A pointer to the page directory to initialize.
 * Setup our paging directory, load it into the cr3 register, copy the kernel heap into the higher half, and enable paging.
 */
void install_paging(/*uint32_t memory_size*/) {

	uint32_t page_directory[1024] __attribute__((aligned(4096)));
	for (int i = 0; i < 1024; i++) {
		// This sets the following flags to the pages:
		//   Supervisor: Only kernel-mode can access them
		//   Write Enabled: It can be both read from and written to
		//   Not Present: The page table is not present
		page_directory[i] = 0x00000002;
	}

	uint32_t first_page_table[1024] __attribute__((aligned(4096)));

	//we will fill all 1024 entries in the table, mapping 4 megabytes.
	// I is where we place the first page table. In this case we place it at 0;
	for (uint32_t i = 0; i < 1024; i++) {
		// As the address is page aligned, it will always leave 12 bits zeroed.
		// Those bits are used by the attributes ;)
		first_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
	}

	// attributes: supervisor level, read/write, present
	page_directory[0] = ((uint32_t)first_page_table) | 3;

	register_interrupt_handler(14, page_fault_handler);

	// Pass the pointer to the page directory to the cr3 register and then enable paging.
	cr3_load_paging_directory(&page_directory);
	cr0_enable_paging();

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
