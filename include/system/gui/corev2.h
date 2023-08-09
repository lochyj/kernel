#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "system/misc/multiboot.h"

typedef struct {
   uint8_t* framebuffer_pointer;
   uint32_t framebuffer_pitch;
   uint32_t framebuffer_width;
   uint32_t framebuffer_height;
   uint32_t framebuffer_bpp;
} vesa_info_t;