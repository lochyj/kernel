#include "system/sys/gdt.h"
//#include "system/sys/tss.h"

gdt_entry_t gdt[MAX_ENTRIES];
gdt_pointer_t gdt_ptr;

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low       =  base & 0xFFFF;
    gdt[num].base_middle    =  (base >> 16) & 0xFF;
    gdt[num].base_high      =  (base >> 24) & 0xFF;

    gdt[num].limit_low      =  limit & 0xFFFF;
    gdt[num].granularity    =  (limit >> 16) & 0x0F;
    
    gdt[num].granularity    |= gran & 0xF0;
    gdt[num].access         =  access;
}

void gdt_init() {
    gdt_ptr.limit   = (sizeof(gdt_entry_t) * MAX_ENTRIES) - 1;
    gdt_ptr.base    = (uint32_t) &gdt;

    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    //tss_write(5, 0x10, 0x0);

    gdt_flush((uint32_t) &gdt_ptr);
    //tss_flush();
}
