#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "system/misc/ports.h"
#include "system/cpu/interrupts.h"
#include "system/debug/debug.h"

typedef struct page {
	uint32_t present	: 1;
	uint32_t rw			: 1;
	uint32_t user		: 1;
	uint32_t accessed	: 1;
	uint32_t dirty		: 1;
	uint32_t unused		: 7;
	uint32_t frame		: 20;
} __attribute__((packed)) page_t;

typedef struct page_table {
	page_t pages[1024];
} page_table_t;

typedef struct page_directory {
	page_table_t *tables[1024];			/* 1024 pointers to page tables */
	uintptr_t physical_tables[1024];	/* Physical addresses of the tables */
	uintptr_t physical_address;			/* The physical address of physical_tables */

	int32_t ref_count;
} page_directory_t;

uintptr_t kmalloc(size_t size);

extern void set_frame(uintptr_t frame_addr);
extern void clear_frame(uintptr_t frame_addr);
extern uint32_t test_frame(uintptr_t frame_addr);
extern uint32_t first_frame();

void page_fault(registers_t *r);

void heap_install();

void* sbrk(uintptr_t increment);

page_t *get_page(uintptr_t address, int make, page_directory_t * dir);

void switch_page_directory(page_directory_t * dir);

void debug_print_directory();

void paging_install(uint32_t memsize);

uintptr_t memory_total();

uintptr_t memory_use();

void free_frame(page_t *page);

void dma_frame(page_t *page, int is_kernel, int is_writeable, uintptr_t address);

void alloc_frame(page_t *page, int is_kernel, int is_writeable);

page_directory_t* clone_directory(page_directory_t * src);

page_table_t* clone_table(page_table_t * src,uintptr_t * physAddr);
