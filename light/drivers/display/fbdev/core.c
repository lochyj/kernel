#include "drivers/display/core.h"

framebuffer_info_t fb_info;

// ----------------|
// Initializations |
// ----------------|


// We initialise the framebuffer info struct with data from the multiboot header, allocate a buffer for the copy framebuffer, and clear both the copy and the real framebuffer
void initialise_framebuffer(multiboot_info_t *multiboot_header) {

    fb_info.framebuffer_pointer = (uint8_t*)(int32_t)(multiboot_header->framebuffer_addr);

    fb_info.framebuffer_pitch = multiboot_header->framebuffer_pitch;

    fb_info.framebuffer_bpp = multiboot_header->framebuffer_bpp;

    fb_info.framebuffer_width = multiboot_header->framebuffer_width;
    fb_info.framebuffer_height = multiboot_header->framebuffer_height;

    // Clear the buffer and make it dark gray
    buffer_clear(0x5A5A5A);

}

// Allocate memory for the framebuffer,
void initialize_render_context(render_context_t* ctx, uint32_t width, uint32_t height, uint32_t x, uint32_t y) {

    ctx->width = width;
    ctx->height = height;

    ctx->x = x;
    ctx->y = y;

    // Allocate the buffer. In future, use malloc, not kmalloc, its just that we dont have malloc yet... Working on it!
    ctx->buffer = (uint32_t*) kmalloc(width * height * sizeof(uint32_t));

    // Clear the buffer as i might still have garbage in it
    memset(ctx->buffer, 0, width * height * sizeof(uint32_t));

}

// ----------|
// Utilities |
// ----------|

void fb_put_pixel(uint32_t x, uint32_t y, uint32_t color) {

    if (x >= fb_info.framebuffer_width || y >= fb_info.framebuffer_height)
        return;

    uint8_t r = color >> 16;
    uint8_t g = color >> 8;
    uint8_t b = color;

    uint32_t offset = x * (fb_info.framebuffer_bpp / 8) + y * fb_info.framebuffer_pitch;

    fb_info.framebuffer_pointer[offset + 0] = b;
    fb_info.framebuffer_pointer[offset + 1] = g;
    fb_info.framebuffer_pointer[offset + 2] = r;

}

void ctx_put_pixel(render_context_t* ctx, uint32_t x, uint32_t y, uint32_t color) {

    if (x >= ctx->width || y >= ctx->height)
        return;

    ctx->buffer[x + (y * ctx->width)] = color;

}

void draw_rect(render_context_t* ctx, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {

    for (uint32_t i = 0; i < width; i++) {
        for (uint32_t j = 0; j < height; j++) {
            ctx_put_pixel(ctx, x + i, y + j, color);
        }
    }

}

void draw_circle(render_context_t* ctx, uint32_t x, uint32_t y, uint32_t radius, uint32_t color) {

    for (uint32_t i = 0; i < radius * 2; i++) {
        for (uint32_t j = 0; j < radius * 2; j++) {
            if ((i - radius) * (i - radius) + (j - radius) * (j - radius) <= radius * radius) {
                ctx_put_pixel(ctx, x + i - radius, y + j - radius, color);
            }
        }
    }

}

void draw_line(render_context_t* ctx, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t width, uint32_t color) {

    int dx = abs((int)x2 - (int)x1);
    int dy = abs((int)y2 - (int)y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    //TODO: add a max iters to this loop.
    for (;;) {
        // Draw the current pixel
        ctx_put_pixel(ctx, x1, y1, color);

        // If the line is wide, draw additional pixels along its width
        for (int w = 0; w < width; w++) {
            if (x1 + w < ctx->width && y1 < ctx->height) {
                ctx_put_pixel(ctx, x1 + w, y1, color);
            }
        }

        // Break if we've reached the end point
        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void draw_char(render_context_t* ctx, uint32_t x, uint32_t y, char c, uint32_t color) {
    //TODO: add scale.

    for (uint32_t i = 0; i < 8; i++) {
        for (uint32_t j = 0; j < 8; j++) {
            if (font_16x16_basic[(uint8_t)c][i] & (1 << j)) {
                ctx_put_pixel(ctx, x + i, y + j, color);
            }
        }
    }

}

// ---------------|
// Render context |
// ---------------|

// TODO: make this draw to a screen instead of the framebuffer and make it update the fb when done.
void draw_render_context(render_context_t* ctx) {

    // Copy the buffer to the framebuffer
    for (uint32_t i = 0; i < ctx->width; i++) {
        for (uint32_t j = 0; j < ctx->height; j++) {
            fb_put_pixel(ctx->x + i, ctx->y + j, ctx->buffer[i + (j * ctx->width)]);
        }
    }

}
