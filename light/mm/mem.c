#include "light/mm/mem.h"

// ----------------------|
// Variables and Helpers |
// ----------------------|

extern void copy_page_physical(uint32_t, uint32_t);

uint32_t *frames;
uint32_t nframes;

#define KERNEL_HEAP_END 0x02000000

#define INDEX_FROM_BIT(b) (b / 0x20)
#define OFFSET_FROM_BIT(b) (b % 0x20)

extern void* end;
uintptr_t placement_pointer = (uintptr_t) &end;
uintptr_t heap_end = (uintptr_t) NULL;

page_directory_t *kernel_directory;
page_directory_t *current_directory;


// --------|
// Kmalloc |
// --------|

void* kmalloc_real(size_t size, int align, uintptr_t* phys) {
	if (heap_end != NULL) {
		if (align && (heap_end & 0xFFFFF000)) {
			heap_end &= 0xFFFFF000;
			heap_end += 0x1000;
		}
		if (phys) {
			*phys = heap_end;
		}
		uintptr_t address = heap_end;
		heap_end += size;
		return (uintptr_t) address;
	}

	if (align && (placement_pointer & 0xFFFFF000)) {
		placement_pointer &= 0xFFFFF000;
		placement_pointer += 0x1000;
	}
	if (phys) {
		*phys = placement_pointer;
	}
	void* address = placement_pointer;
	placement_pointer += size;
	return (void*)address;
}

void* kmalloc(size_t size) {

	return kmalloc_real(size, 0, NULL);

}

void* kvmalloc(size_t size) {

	return kmalloc_real(size, 1, NULL);

}

void* kmalloc_p(size_t size, uintptr_t *phys) {

	return kmalloc_real(size, 0, phys);

}

void* kvmalloc_p(size_t size, uintptr_t *phys) {

	return kmalloc_real(size, 1, phys);

}


// -----------------|
// Frame allocation |
// -----------------|

void set_frame(uintptr_t frame_addr) {

	uint32_t frame  = frame_addr / 0x1000;

	uint32_t index  = INDEX_FROM_BIT(frame);
	uint32_t offset = OFFSET_FROM_BIT(frame);

	frames[index] |= (0x1 << offset);

}

void clear_frame(uintptr_t frame_addr) {

	uint32_t frame  = frame_addr / 0x1000;

	uint32_t index  = INDEX_FROM_BIT(frame);
	uint32_t offset = OFFSET_FROM_BIT(frame);

	frames[index] &= ~(0x1 << offset);

}

uint32_t test_frame(uintptr_t frame_addr) {

	uint32_t frame  = frame_addr / 0x1000;

	uint32_t index  = INDEX_FROM_BIT(frame);
	uint32_t offset = OFFSET_FROM_BIT(frame);

	return (frames[index] & (0x1 << offset));

}

uint32_t first_frame() {

	uint32_t i, j;

	for (i = 0; i < INDEX_FROM_BIT(nframes); ++i) {
		if (frames[i] != 0xFFFFFFFF) {
			for (j = 0; j < 32; ++j) {
				uint32_t testFrame = 0x1 << j;
				if (!(frames[i] & testFrame)) {
					return i * 0x20 + j;
				}
			}
		}
	}

	printf("WARNING: System claims to be out of usable memory, which means we probably overwrote the page frames.\n");

}

void alloc_frame(page_t *page, int is_kernel, int is_writeable) {

	if (page->frame != 0) {

		page->present = 1;
		page->rw      = (is_writeable == 1) ? 1 : 0;
		page->user    = (is_kernel == 1)    ? 0 : 1;

		return;
	} else {

		uint32_t index = first_frame();

		ASSERT(index != (uint32_t)-1 /*&& "Out of frames."*/);

		set_frame(index * 0x1000);

		page->present = 1;
		page->rw      = (is_writeable == 1) ? 1 : 0;
		page->user    = (is_kernel == 1)    ? 0 : 1;
		page->frame   = index;

	}

}

void dma_frame(page_t *page, int is_kernel, int is_writeable, uintptr_t address) {

	/* Page this address directly */
	page->present = 1;
	page->rw      = (is_writeable) ? 1 : 0;
	page->user    = (is_kernel)    ? 0 : 1;
	page->frame   = address / 0x1000;

}

void free_frame(page_t *page) {

	uint32_t frame;

	if (!(frame = page->frame)) {

		ASSERT(0);

		return;
	} else {

		clear_frame(frame * 0x1000);
		page->frame = 0x0;

	}
}


// ------------------|
// Utility functions |
// ------------------|

uintptr_t memory_use() {

	uintptr_t ret = 0;
	uint32_t i, j;

	for (i = 0; i < INDEX_FROM_BIT(nframes); ++i) {

		for (j = 0; j < 32; ++j) {

			uint32_t testFrame = 0x1 << j;

			if (frames[i] & testFrame) {

				ret++;

			}

		}

	}

	return ret * 4;

}

uintptr_t memory_total() {

	return nframes * 4;

}


// -------|
// Paging |
// -------|

void paging_install(uint32_t memsize) {

	nframes = memsize  / 4;
	frames  = (uint32_t *)kmalloc(INDEX_FROM_BIT(nframes * 8));

	memset(frames, 0, INDEX_FROM_BIT(nframes));

	uintptr_t phys;

	kernel_directory = (page_directory_t *)kvmalloc_p(sizeof(page_directory_t),&phys);

	memset(kernel_directory, 0, sizeof(page_directory_t));

	uint32_t i = 0;

	while (i < placement_pointer + 0x3000) {

		alloc_frame(get_page(i, 1, kernel_directory), 1, 0);
		i += 0x1000;

	}

	/* XXX VGA TEXT MODE VIDEO MEMORY EXTENSION */
	for (uint32_t j = 0xb8000; j < 0xc0000; j += 0x1000) {

		alloc_frame(get_page(j, 1, kernel_directory), 0, 1);

	}

	register_interrupt_handler(14, page_fault);

	kernel_directory->physical_address = (uintptr_t)kernel_directory->physical_tables;

	/* Kernel Heap Space */
	for (i = placement_pointer; i < KERNEL_HEAP_END; i += 0x1000) {
		alloc_frame(get_page(i, 1, kernel_directory), 1, 0);
	}

	current_directory = clone_directory(kernel_directory);

	switch_page_directory(kernel_directory);

	heap_install();

}

/*
 * Clone a page directory and its contents.
 * (If you do not intend to clone the contents, do it yourself!)
 *
 * @param  src Pointer to source directory to clone from.
 * @return A pointer to a new directory.
 */
page_directory_t* clone_directory(page_directory_t * src) {

	/* Allocate a new page directory */
	uintptr_t phys;
	page_directory_t * dir = (page_directory_t *)kvmalloc_p(sizeof(page_directory_t), &phys);

	/* Clear it out */
	memset(dir, 0, sizeof(page_directory_t));
	dir->ref_count = 1;

	/* Calculate the physical address offset */
	uintptr_t offset = (uintptr_t)dir->physical_tables - (uintptr_t)dir;

	/* And store it... */
	dir->physical_address = phys + offset;

	uint32_t i;

	for (i = 0; i < 1024; ++i) {

		/* Copy each table */
		if (!src->tables[i] || (uintptr_t)src->tables[i] == (uintptr_t)0xFFFFFFFF)
			continue;

		if (kernel_directory->tables[i] == src->tables[i]) {

			/* Kernel tables are simply linked together */
			dir->tables[i] = src->tables[i];
			dir->physical_tables[i] = src->physical_tables[i];

		} else {

			if (i * 0x1000 * 1024 < 0x20000000) {

				/* User tables must be cloned */
				uintptr_t phys;

				dir->tables[i] = clone_table(src->tables[i], &phys);
				dir->physical_tables[i] = phys | 0x07;

			}

		}

	}

	return dir;

}

/*
 * Clone a page table
 *
 * @param src      Pointer to a page table to clone.
 * @param physAddr [out] Pointer to the physical address of the new page table
 * @return         A pointer to a new page table.
 */
page_table_t* clone_table(page_table_t* src, uintptr_t* physAddr) {

	/* Allocate a new page table */
	page_table_t * table = (page_table_t *)kvmalloc_p(sizeof(page_table_t), physAddr);

	memset(table, 0, sizeof(page_table_t));

	uint32_t i;

	for (i = 0; i < 1024; ++i) {

		/* For each frame in the table... */
		if (!src->pages[i].frame) {
			continue;
		}

		/* Allocate a new frame */
		alloc_frame(&table->pages[i], 0, 0);

		/* Set the correct access bit */
		if (src->pages[i].present)	table->pages[i].present = 1;
		if (src->pages[i].rw)		table->pages[i].rw = 1;
		if (src->pages[i].user)		table->pages[i].user = 1;
		if (src->pages[i].accessed)	table->pages[i].accessed = 1;
		if (src->pages[i].dirty)	table->pages[i].dirty = 1;

		/* Copy the contents of the page from the old table to the new one */
		copy_page_physical(src->pages[i].frame * 0x1000, table->pages[i].frame * 0x1000);

	}

	return table;

}

void switch_page_directory(page_directory_t * dir) {

	current_directory = dir;

	asm volatile ("mov %0, %%cr3":: "r"(dir->physical_address));

	uint32_t cr0;

	asm volatile ("mov %%cr0, %0": "=r"(cr0));

	cr0 |= 0x80000000;

	asm volatile ("mov %0, %%cr0":: "r"(cr0));

}

page_t *get_page(uintptr_t address, int make, page_directory_t * dir) {

	address /= 0x1000;
	uint32_t table_index = address / 1024;

	if (dir->tables[table_index]) {

		return &dir->tables[table_index]->pages[address % 1024];

	} else if(make) {

		uint32_t temp;

		dir->tables[table_index] = (page_table_t *)kvmalloc_p(sizeof(page_table_t), (uintptr_t *)(&temp));

		memset(dir->tables[table_index], 0, sizeof(page_table_t));

		dir->physical_tables[table_index] = temp | 0x7; /* Present, R/w, User */

		return &dir->tables[table_index]->pages[address % 1024];

	} else {

		return NULL;

	}

}

void page_fault(registers_t *r) {

	uint32_t faulting_address;

	asm volatile("mov %%cr2, %0" : "=r"(faulting_address));

	int present  = !(r->err_code & 0x1) ? 1 : 0;
	int rw       = r->err_code & 0x2    ? 1 : 0;
	int user     = r->err_code & 0x4    ? 1 : 0;
	int reserved = r->err_code & 0x8    ? 1 : 0;
	int id       = r->err_code & 0x10   ? 1 : 0;

	printf("Segmentation fault. (p:%d,rw:%d,user:%d,res:%d,id:%d) at 0x%x eip:0x%x" /*pid=%d,%d [%s]\n"*/,
			present, rw, user, reserved, id, faulting_address, r->eip /*, current_process->id, current_process->group, current_process->name*/);

	PANIC("Page fault");

}

void heap_install() {

	printf("Initializing block allocator...");

	heap_end = (placement_pointer + 0x1000) & ~0xFFF;

}