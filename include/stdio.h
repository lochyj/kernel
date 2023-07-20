#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "drivers/console.h"

static int* printf_number(int* argp, int length, int sign);
void printf(const char* format, ...);
