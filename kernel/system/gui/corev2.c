#include "system/gui/corev2.h"

framebuffer_info_t FRAMEBUFFER_INFO;


// We initialise the framebuffer info struct with data from the multiboot header, allocate a buffer for the copy framebuffer, and clear both the copy and the real framebuffer
void initialise_VBE(multiboot_info_t *multiboot_header) {

   FRAMEBUFFER_INFO.framebuffer_pointer = (uint8_t*)(int32_t)(multiboot_header->framebuffer_addr);

   FRAMEBUFFER_INFO.framebuffer_pitch = multiboot_header->framebuffer_pitch;

   FRAMEBUFFER_INFO.framebuffer_bpp = multiboot_header->framebuffer_bpp;

   FRAMEBUFFER_INFO.framebuffer_width = multiboot_header->framebuffer_width;
   FRAMEBUFFER_INFO.framebuffer_height = multiboot_header->framebuffer_height;

   FRAMEBUFFER_INFO.framebuffer_pointer_duplicate = (uint8_t*) kmalloc(multiboot_header->framebuffer_width * multiboot_header->framebuffer_height * (multiboot_header->framebuffer_bpp / 8));

   // Clear the buffer and make it dark gray
   buffer_clear(0x5A5A5A);

}

void draw_buffer_copy_to_global() {

   for (uint32_t x = 0; x < FRAMEBUFFER_INFO.framebuffer_width; x++) {
      for (uint32_t y = 0; y < FRAMEBUFFER_INFO.framebuffer_height; y++) {
         put_pixel_global(x, y, FRAMEBUFFER_INFO.framebuffer_pointer_duplicate[x + (y * FRAMEBUFFER_INFO.framebuffer_width)]);
      }
   }

}

void put_pixel_global(uint32_t x, uint32_t y, uint32_t color) {

   if (x >= FRAMEBUFFER_INFO.framebuffer_width || y >= FRAMEBUFFER_INFO.framebuffer_height)
      return;

   uint8_t r = color >> 16;
   uint8_t g = color >> 8;
   uint8_t b = color;

   uint32_t offset = x * (FRAMEBUFFER_INFO.framebuffer_bpp / 8) + y * FRAMEBUFFER_INFO.framebuffer_pitch;

   FRAMEBUFFER_INFO.framebuffer_pointer_duplicate[offset + 0] = b;
   FRAMEBUFFER_INFO.framebuffer_pointer_duplicate[offset + 1] = g;
   FRAMEBUFFER_INFO.framebuffer_pointer_duplicate[offset + 2] = r;

}

void put_pixel_global_copy(uint32_t x, uint32_t y, uint32_t color) {

   if (x >= FRAMEBUFFER_INFO.framebuffer_width || y >= FRAMEBUFFER_INFO.framebuffer_height)
      return;

   uint8_t r = color >> 16;
   uint8_t g = color >> 8;
   uint8_t b = color;

   uint32_t offset = x * (FRAMEBUFFER_INFO.framebuffer_bpp / 8) + y * FRAMEBUFFER_INFO.framebuffer_pitch;

   FRAMEBUFFER_INFO.framebuffer_pointer_duplicate[offset + 0] = b;
   FRAMEBUFFER_INFO.framebuffer_pointer_duplicate[offset + 1] = g;
   FRAMEBUFFER_INFO.framebuffer_pointer_duplicate[offset + 2] = r;

}

void buffer_clear(uint32_t colour) {

   for (uint32_t x = 0; x < FRAMEBUFFER_INFO.framebuffer_width; x++) {
      for (uint32_t y = 0; y < FRAMEBUFFER_INFO.framebuffer_height; y++) {
         put_pixel_global(x, y, colour);
      }
   }
}


// ---

void render_context_to_global(render_context_t* ctx, uint32_t offset_x, uint32_t offset_y) {

   for (uint32_t x = 0; x < ctx->width; x++) {
      for (uint32_t y = 0; y < ctx->height; y++) {
         put_pixel_global_copy(x + ctx->x + offset_x, y + ctx->y + offset_y, ctx->buffer[x + (y * ctx->width)]);
      }
   }

}

void initialize_render_context(render_context_t* ctx, uint32_t width, uint32_t height, uint32_t x, uint32_t y /*, uint8_t order*/) {

   ctx->width = width;
   ctx->height = height;

   ctx->x = x;
   ctx->y = y;

   ctx->buffer = (uint32_t*) kmalloc(width * height * sizeof(uint32_t));

   // ctx->order = order;

}

int put_pixel(render_context_t* ctx, uint32_t x, uint32_t y, uint32_t colour /* Don't complain about the spelling ok... */) {

   // This is probably too slow... Move it somewhere that wont be as slow but still works
   //ASSERT(ctx->width > x && ctx->height > y && x >= 0 && y >= 0 && ctx->buffer != NULL);

   ctx->buffer[x + y * ctx->width] = colour;

   return 0;

}

int draw_rect(render_context_t* ctx, rect_t* rect) {

   // if (rect->has_decoration) {
   //    PANIC("Decoration isn't implemented yet");
   //    return 1;
   // }

   for (uint32_t x = 0; x < rect->width; x++) {
      for (uint32_t y = 0; y < rect->height; y++) {
         put_pixel(ctx, x + rect->x, y + rect->y, rect->colour);
      }
   }

   return 0;
}

// --- BEGIN TEMP

void global_draw_rect(uint32_t x_offset, uint32_t y_offset, uint32_t width, uint32_t height, uint32_t colour) {

   for (uint32_t x = 0; x < width; x++) {
      for (uint32_t y = 0; y < height; y++) {
         put_pixel_global_copy(x + x_offset, y + y_offset, colour);
      }
   }

}

// void global_draw_line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t colour) {

//    uint32_t dx, dy, p, x, y;

//    dx = x1 - x0;
//    dy = y1 - y0;

//    x = x0;
//    y = y0;

//    p = 2 * dy - dx;

//    while(x<x1) {

//       if (p >= 0) {

//          put_pixel_global_copy(x, y, colour);

//          y = y + 1;
//          p = p + 2 * dy - 2 * dx;

//       } else {

//          put_pixel_global_copy(x, y, colour);

//          p = p + 2 * dy;

//       }

//       x = x + 1;
//    }

// }



// END TEMP ---


