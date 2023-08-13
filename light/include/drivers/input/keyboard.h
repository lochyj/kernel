#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "light/irq/interrupts.h"
#include "asm/ports.h"

#define SC_MAX 57

void init_keyboard();
