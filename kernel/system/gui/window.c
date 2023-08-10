#include "system/gui/corev2.h"

void draw_window_decoration(window_t* window) {
   if(window->y < 25) {

      window->y = 25;

   } else if (window->y + window->height > FRAMEBUFFER_INFO.framebuffer_height) {

      window->y = FRAMEBUFFER_INFO.framebuffer_height - (window->height + 1);

   } else if (window->x + window->width > FRAMEBUFFER_INFO.framebuffer_width) {

      window->x = FRAMEBUFFER_INFO.framebuffer_width - (window->width + 1);

   } else if (window->x < 1) {

      window->x = 1;

   }

   global_draw_rect(window->x - 1, window->y - 25, window->width + 2, 25, 0x000000);

   global_draw_rect(window->x + window->width, window->y, 1, window->height, 0x000000);

   global_draw_rect(window->x - 1, window->y, 1, window->height, 0x000000);

      global_draw_rect(window->x - 1, window->y + window->height, window->width + 2, 1, 0x000000);


}

void render_window(window_t* window) {

   draw_window_decoration(window);
   render_context_to_global(window->ctx, window->x, window->y);

   draw_buffer_copy_to_global();

}

window_t* create_window(uint32_t x, uint32_t y, uint32_t width, uint32_t height, char* title) {
   window_t* window = (window_t*) kmalloc(sizeof(window_t));

   window->ctx = (render_context_t*) kmalloc(sizeof(render_context_t));

   window->x = x;
   window->y = y;

   window->width = width;
   window->height = height;

   window->title = title;

   // TODO: add the mask functionality
   // window->mask->has_decoration = false;

   initialize_render_context(window->ctx, 200, 200, 0, 0);

   return window;

}
