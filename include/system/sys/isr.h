#pragma once 

#include "system/debug.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct regs {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

typedef void (*isr_t)(registers_t*);

void isr_install();

void fault_handler(registers_t *r);
