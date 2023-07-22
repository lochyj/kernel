#pragma once

#include <stdint.h>
#include <stdbool.h>

// Global Descriptor Table Entry

struct gdt_entry {
	/* Limits */
	unsigned short limit_low;
	/* Segment address */
	unsigned short base_low;
	unsigned char base_middle;
	/* Access modes */
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __attribute__((packed));


// GDT pointer
struct gdt_ptr {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

/**
 * (ASM) gdt_flush
 * Reloads the segment registers
 */
extern void gdt_flush();

void gdt_install();