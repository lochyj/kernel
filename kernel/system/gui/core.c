#include "system/gui/core.h"

vesa_info_t VESA_INFO;

void put_pixel(uint32_t x, uint32_t y, uint32_t color) {

   if (x >= VESA_INFO.framebuffer_width || y >= VESA_INFO.framebuffer_height)
      return;

   uint8_t r = color >> 16;
   uint8_t g = color >> 8;
   uint8_t b = color;

   uint32_t where = x * (VESA_INFO.framebuffer_bpp / 8) + y * VESA_INFO.framebuffer_pitch;

   VESA_INFO.framebuffer_pointer[where + 0] = b;
   VESA_INFO.framebuffer_pointer[where + 1] = g;
   VESA_INFO.framebuffer_pointer[where + 2] = r;

}

void initialise_VBE(multiboot_info_t *multiboot_header) {

   VESA_INFO.framebuffer_pointer = (uint8_t*)(int32_t)(multiboot_header->framebuffer_addr);

   VESA_INFO.framebuffer_pitch = multiboot_header->framebuffer_pitch;

   VESA_INFO.framebuffer_bpp = multiboot_header->framebuffer_bpp;

   VESA_INFO.framebuffer_width = multiboot_header->framebuffer_width;
   VESA_INFO.framebuffer_height = multiboot_header->framebuffer_height;

}

// TODO: optimise this function so it doesnt call put_pixel for every pixel.
void draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t colour) {

   for (uint32_t i = x; i < x + width; i++) {
      for (uint32_t j = y; j < y + height; j++) {
         put_pixel(i, j, colour);
      }
   }

}

void draw_character(uint32_t console_x, uint32_t console_y, uint32_t colour, font_t* font, char character) {
   for (uint32_t i = 0; i < 16; i++) {

      for (uint32_t j = 0; j < 8; j++) {

         if (blink_default.data[i][j][character]) {
            put_pixel(console_x + j, console_y + i, colour);
         }

      }

   }
}
