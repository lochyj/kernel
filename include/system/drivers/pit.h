#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "system/misc/ports.h"
#include "system/cpu/interrupts.h"

void register_PIT();
void begin_timer(uint32_t frequency);