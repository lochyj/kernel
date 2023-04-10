#include "system/sys/idt.h"
#include "string.h"

idt_entry_t idt[IDT_ENTRIES_LENGTH];
idt_pointer_t idt_ptr;

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
	idt[num].low_offset 	= (base & 0xFFFF);
	idt[num].high_offset 	= (base >> 16) & 0xFFFF;

	idt[num].sel 			= sel;
	idt[num].always0 		= 0;
	idt[num].flags 			= flags;
}

/* Installs the IDT */
void idt_install() {
    idt_ptr.limit 			= (sizeof (idt_entry_t) * IDT_ENTRIES_LENGTH) - 1;
    idt_ptr.base 			= (uint32_t) &idt;

    memset((void*) &idt, 0, sizeof(idt_entry_t) * IDT_ENTRIES_LENGTH);

    idt_load();
}