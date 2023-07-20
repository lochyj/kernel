#include "system/gdt.h"

struct gdt_entry	gdt[6];
struct gdt_ptr		gp;

/**
 * Set a GDT descriptor
 *
 * @param num The number for the descriptor to set.
 * @param base Base address
 * @param limit Limit
 * @param access Access permissions
 * @param gran Granularity
 */
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
	/* Base Address */
	gdt[num].base_low =		(base & 0xFFFF);
	gdt[num].base_middle =	(base >> 16) & 0xFF;
	gdt[num].base_high =	(base >> 24) & 0xFF;
	/* Limits */
	gdt[num].limit_low =	(limit & 0xFFFF);
	gdt[num].granularity =	(limit >> 16) & 0X0F;
	/* Granularity */
	gdt[num].granularity |= (gran & 0xF0);
	/* Access flags */
	gdt[num].access = access;
}

void gdt_install() {
	/* GDT pointer and limits */
	gp.limit = (sizeof(struct gdt_entry) * 6) - 1;
	gp.base = (unsigned int)&gdt;
	/* NULL */
	gdt_set_gate(0, 0, 0, 0, 0);
	/* Code segment */
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	/* Data segment */
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // maybe set to 0xFFFFF

	gdt_flush();
}