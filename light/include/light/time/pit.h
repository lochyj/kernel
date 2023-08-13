#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "asm/ports.h"
#include "light/irq/interrupts.h"

void register_PIT();
void begin_timer(uint32_t frequency);