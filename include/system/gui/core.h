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

void put_pixel(uint32_t x, uint32_t y, uint32_t colour);
void initialise_VBE(multiboot_info_t* multiboot_header);

void draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t colour);
void draw_character(uint32_t x, uint32_t y, uint32_t scale, uint32_t colour, uint8_t character);

void vga_print_string(char* string);

void draw_rounded_rectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t radius, uint32_t color);