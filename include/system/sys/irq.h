#pragma once

#include "system/sys/isr.h"
#include "system/sys/idt.h"
#include "misc/ports.h"
#include <string.h>

void irq_install_handler(int irq, isr_t handler);

void irq_uninstall_handler(int irq);

void irq_install();

void irq_handler(registers_t *r);