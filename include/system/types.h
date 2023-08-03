#pragma once

#include <stdint.h>

#define FOUR_KB     0x1000
#define ONE_MB      0x100000
#define FOUR_MB     0x400000
#define EIGHT_MB    0x800000

typedef unsigned long size_t;

typedef uint32_t physaddr_t;

typedef struct {
    uint32_t kernel_physical_start;
    uint32_t kernel_physical_end;
    uint32_t kernel_virtual_start;
    uint32_t kernel_virtual_end;
} __attribute__((packed)) kernel_memory_info_t;
