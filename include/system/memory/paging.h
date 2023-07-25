#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "system/memory/kheap.h"
#include "system/misc/ports.h"
#include "system/cpu/interrupts.h"
#include "system/debug/debug.h"

typedef struct page {
   uint32_t present    : 1;   // Page present in memory
   uint32_t rw         : 1;   // Read-only if clear, readwrite if set
   uint32_t user       : 1;   // Supervisor level only if clear
   uint32_t accessed   : 1;   // Has the page been accessed since last refresh?
   uint32_t dirty      : 1;   // Has the page been written to since last refresh?
   uint32_t unused     : 7;   // Amalgamation of unused and reserved bits
   uint32_t frame      : 20;  // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table {
   page_t pages[1024];
} page_table_t;

typedef struct page_directory {
   page_table_t *tables[1024];      // Array of pointers to page-tables.
   uint32_t table_physical[1024];   // Array of pointers to the page-tables above, but gives their *physical* location, for loading into the CR3 register.
   uint32_t address_physical;
} page_directory_t __attribute__((aligned(4096)));

void page_fault_handler(registers_t* regs);

void install_paging();
