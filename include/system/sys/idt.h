#pragma once

#include <stdint.h>

#define KERNEL_CS 0x08
#define IDT_ENTRIES_LENGTH 256

typedef struct idt_entry {
    uint8_t     always0;
    uint8_t     flags;
    uint16_t    low_offset;
    uint16_t    sel;
    uint16_t    high_offset;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_pointer {
    uint16_t    limit;
    uint32_t    base;
} __attribute__((packed)) idt_pointer_t;

extern void idt_load();
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_install();