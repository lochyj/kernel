#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "system/misc/multiboot.h"
#include "system/debug/debug.h"
#include "system/memory/mem.h"

typedef struct {
   uint8_t r;
   uint8_t g;
   uint8_t b;
   uint8_t a;
} rgba_t;

typedef struct {
   uint8_t* framebuffer_pointer;

   uint8_t* framebuffer_pointer_duplicate;

   uint32_t framebuffer_pitch;

   uint32_t framebuffer_width;
   uint32_t framebuffer_height;

   uint32_t framebuffer_bpp;

} framebuffer_info_t;

typedef struct {
   uint32_t width;
   uint32_t height;

   uint32_t x;          // This is the current x position of the top left corner of the render context in the larger framebuffer
   uint32_t y;          // This is the current y position of the top left corner of the render context in the larger framebuffer

   // This will need to be alloc'd by the initializer. It is simply width * height * sizeof(uint32_t) !!! I THINK !!! I'm probably wrong though
   uint32_t* buffer;     // This is the buffer that is read from and drawn onto the main framebuffer

   uint8_t order;       // Z stacking order. Higher numbers are drawn on top of lower numbers
} render_context_t;

typedef struct {
   uint32_t border_colour;
   uint32_t border_width;
} border_t;

typedef struct {
   uint32_t radius;
} rounded_t;

typedef struct {
   uint32_t radius;
   uint32_t border_colour;
   uint32_t border_width;
} rounded_border_t;

typedef struct {
   uint32_t x;
   uint32_t y;

   uint32_t width;
   uint32_t height;

   uint32_t colour;

   // bool has_decoration;
   // union {
   //    border_t border;
   //    rounded_t rounded;
   //    rounded_border_t rounded_border;
   // } decoration;

} rect_t;


typedef struct {

   // These values are relative to the render context.
   uint32_t x;
   uint32_t y;

   uint32_t radius;

   uint32_t colour;

   // bool has_decoration;
   // border_t border;

} sphere_t;

// TODO: move this to a new file.

typedef struct {

   uint32_t width;
   uint32_t height;

   render_context_t* ctx;

   char* title;

   uint32_t colour;

} window_decoration_t;

typedef struct {

   uint32_t x;
   uint32_t y;

   uint32_t width;
   uint32_t height;

   rect_t* mask;           // The mask is used to trim the window to a certain shape. Essentially a rect that is &-ed with the render context to cut it to shape. e.g: rounding.
   render_context_t* ctx;

   char* title;

} window_t;

extern framebuffer_info_t FRAMEBUFFER_INFO;

// Core
void initialise_VBE(multiboot_info_t *multiboot_header);
void buffer_clear(uint32_t colour);

void draw_buffer_copy_to_global();

void put_pixel_global(uint32_t x, uint32_t y, uint32_t color);
void put_pixel_global_copy(uint32_t x, uint32_t y, uint32_t color);

void render_context_to_global(render_context_t* ctx, uint32_t offset_x, uint32_t offset_y);
void initialize_render_context(render_context_t* ctx, uint32_t width, uint32_t height, uint32_t x, uint32_t y /*, uint8_t order*/);

int put_pixel(render_context_t* ctx, uint32_t x, uint32_t y, uint32_t colour /* Don't complain about the spelling ok... */);

int draw_rect(render_context_t* ctx, rect_t* rect);

void global_draw_rect(uint32_t x_offset, uint32_t y_offset, uint32_t width, uint32_t height, uint32_t colour);

// Window
window_t* create_window(uint32_t x, uint32_t y, uint32_t width, uint32_t height, char* title);
void render_window(window_t* window);

// Cursor
void draw_cursor(uint32_t x, uint32_t y/*, uint32_t scale*/);