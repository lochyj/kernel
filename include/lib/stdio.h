#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#include "system/drivers/console.h"
#include "system/gui/core.h"

void printf(const char* format, ...);
