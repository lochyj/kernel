#pragma once

#include <stdint.h>

#define MAX_ENTRIES 6

// Defines a GDT entry. Packed to prevent compiler from optimizing the packing.
typedef struct gdt_entry {
    uint16_t    limit_low;
    uint16_t    base_low;
    uint8_t     base_middle;
    uint8_t     access;
    uint8_t     granularity;
    uint8_t     base_high;
} __attribute__((packed)) gdt_entry_t;

// Also needs to be packed.
typedef struct gdt_pointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_pointer_t;

extern gdt_flush(uint32_t gdt_ptr);

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
void gdt_init();