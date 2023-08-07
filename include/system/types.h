#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "system/gui/core.h"

typedef unsigned long size_t;

typedef uint32_t physaddr_t;

typedef struct {
   bool is_text_mode;
   vesa_info_t vesa_info;

   uint32_t mem_lower;
   uint32_t mem_upper;

} kernel_info_t;
