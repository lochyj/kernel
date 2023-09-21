#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "drivers/display/core.h"

typedef struct {
    uint32_t width;
    uint32_t height;

    uint32_t x;          // This is the current x position of the top left corner of the render context in the larger framebuffer
    uint32_t y;          // Ditto; except for y position.

    char* title;         // The title of the window

    render_context_t* ctx;
    render_context_t* overflow_ctx; // Gets rendered underneath the main ctx, used for window borders, details and that sort of thing.
} window_t;

window_t* create_window(uint32_t width, uint32_t height, uint32_t x, uint32_t y, char* title);
void destroy_window(window_t* window);

void construct_window_overflow(window_t* window);

void render_window(window_t* window);
