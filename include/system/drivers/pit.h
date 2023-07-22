#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "system/misc/ports.h"
#include "system/cpu/interrupts.h"

void init_timer(uint32_t frequency);