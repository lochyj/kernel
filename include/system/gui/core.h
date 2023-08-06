#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "system/misc/multiboot.h"
#include "system/gui/fonts.h"

typedef struct {
   uint8_t* framebuffer_pointer;
   uint32_t framebuffer_pitch;
   uint32_t framebuffer_width;
   uint32_t framebuffer_height;
   uint32_t framebuffer_bpp;
} vesa_info_t;

void put_pixel(uint32_t x, uint32_t y, uint32_t colour);
void initialise_VBE(multiboot_info_t* multiboot_header);
