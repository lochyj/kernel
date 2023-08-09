#include "system/gui/corev2.h"

static vesa_info_t VESA_INFO;

typedef struct {
   uint32_t width;
   uint32_t height;
   
} render_context_t;

static void put_pixel(uint32_t x, uint32_t y, uint32_t color) {

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
