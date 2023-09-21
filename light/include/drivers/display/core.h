#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "light/multiboot.h"
#include "light/mm/mem.h"

#include "drivers/display/font/16x16basic.h"

typedef struct {
   uint8_t* framebuffer_pointer;

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

   uint32_t* buffer;     // This is the buffer that is read from and drawn onto the main framebuffer. It will need to be alloc'd!

   uint8_t order;       // Z stacking order. Higher numbers are drawn on top of lower numbers
} render_context_t;

void initialise_framebuffer(multiboot_info_t *multiboot_header);
void initialize_render_context(render_context_t* ctx, uint32_t width, uint32_t height, uint32_t x, uint32_t y);

void ctx_put_pixel(render_context_t* ctx, uint32_t x, uint32_t y, uint32_t color);
void draw_rect(render_context_t* ctx, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void draw_circle(render_context_t* ctx, uint32_t x, uint32_t y, uint32_t radius, uint32_t color);
void draw_line(render_context_t* ctx, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t width, uint32_t color);
void draw_char(render_context_t* ctx, uint32_t x, uint32_t y, char c, uint32_t color);

void draw_render_context(render_context_t* ctx);
