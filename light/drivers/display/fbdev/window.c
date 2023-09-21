#include "drivers/display/window.h"

#define WINDOW_BORDER_WIDTH 2
#define WINDOW_HANDLE_HEIGHT 20

window_t* create_window(uint32_t width, uint32_t height, uint32_t x, uint32_t y, char* title) {
    //TODO: replace with malloc.
    window_t* window = (window_t*) kmalloc(sizeof(window_t));

    window->width = width;
    window->height = height;

    window->x = x;
    window->y = y;

    initialize_render_context(window->ctx, width, height, x, y);

    // Some temporary variables to make the code more readable.
    uint32_t overflow_width = width + (WINDOW_BORDER_WIDTH * 2);
    uint32_t overflow_height = height + (WINDOW_BORDER_WIDTH * 2) + WINDOW_HANDLE_HEIGHT;

    uint32_t overflow_x = x - WINDOW_BORDER_WIDTH;
    uint32_t overflow_y = y - WINDOW_BORDER_WIDTH - WINDOW_HANDLE_HEIGHT;

    initialize_render_context(window->overflow_ctx, overflow_width, overflow_height, overflow_x, overflow_y);

    construct_window_overflow(window);

    // Same here.
    // Allocate a buffer for the title. (256 characters should be fine for now)
    window->title = (char*) kmalloc(256 * sizeof(char));

    if (strlen(title) == 0) {
        // If the title is null, just set it to an empty string and terminate it to prevent any errors.
        title[0] = '\0';
    }

    if (strlen(title) > 256) {
        // If the title is too long, just truncate it.
        title[256] = '\0';
    }

    window->title = title;

    return window;
}

void destroy_window(window_t* window) {

    // free((void*)window);

}

void render_window(window_t* window) {

    // Draw the overflow first.
    draw_render_context(window->overflow_ctx);

    // Now we can draw the main context.
    draw_render_context(window->ctx);

}

// Draw the window decoration. (The handle bar and the border)
void construct_window_overflow(window_t* window) {
    // Sorry about this really messy function. I will fix it one day.
    // (Probably need to separate out each function into their own function. The handle bar, the border, title, etc.)

    // Additionally, all positions are relative to the window's position.
    // This is because we are drawing straight to the window's context.

    // Draw the large handlebar
    draw_rect(
            window->overflow_ctx,    // Context
            1,                      // X position
            window->y - (WINDOW_HANDLE_HEIGHT + WINDOW_BORDER_WIDTH),   // Y position
            window->width + 2,      // Width
            (WINDOW_HANDLE_HEIGHT + WINDOW_BORDER_WIDTH),   // Height
            0x000000                // Color
        );

    // Draw the window border
    draw_rect(
            window->overflow_ctx,    // Context
            window->width + WINDOW_BORDER_WIDTH,    // X position
            0,                      // Y position
            1,                      // Width
            window->height,         // Height
            0x000000                // Color
        );

    draw_rect(
            window->overflow_ctx,    // Context
            1,                      // X position
            window->y,              // Y position
            1,                      // Width
            window->height,         // Height
            0x000000                // Color
        );

    draw_rect(
            window->overflow_ctx,    // Context
            1,                      // X position
            window->y + window->height, // Y position
            window->width + 2,      // Width
            1,                      // Height
            0x000000                // Color
        );

    // The font is 16x16 pixels. (For now. I will make it configurable later.)

    uint32_t title_length;

    // Check if the title is too long to fit in the window.
    // (We have 2 pixels of padding on each side. 4 pixels total.)
    // (Padding is on both the x and y directions)
    if (strlen(window->title) * 16 > window->width - 4) {
        title_length = (window->width - 4) / 16;
    } else {
        title_length = strlen(window->title);
    }

    for (uint32_t i = 0; i < title_length; i++) {
        draw_char(
                window->overflow_ctx,   // Context
                2 + (i * 16),           // X position
                window->y - (WINDOW_HANDLE_HEIGHT - 2),   // Y position
                window->title[i],       // Character
                0xFFFFFF                // Color
            );
    }

}
