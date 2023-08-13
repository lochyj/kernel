#include "drivers/display/corev2.h"


/*
    | Binary       | |Hex|

    Cursor white mask
    0011000000000000 3000
    0011100000000000 3800
    0011110000000000 3C00
    0011111000000000 3E00
    0011111100000000 3F00
    0011111110000000 3F80
    0011111111000000 3FC0
    0011111111100000 3FE0
    0011111111110000 3FF0
    0011111111111000 3FF8
    0011111111111100 3FFC
    0011111111111100 3FFC
    0011111111000000 3FC0
    0011110111100000 3DE0
    0011100111110000 39F0
    0011000011111000 30F8

    Cursor black mask
    0011000000000000 3000
    0010100000000000 2800
    0010010000000000 2400
    0010001000000000 2200
    0010000100000000 2100
    0010000010000000 2080
    0010000001000000 2040
    0010000000100000 2020
    0010000000010000 2010
    0010000000001000 2008
    0010000000000100 2004
    0010000001111100 207C
    0010001101000000 2340
    0010110100100000 2D20
    0010100100010000 2910
    0011000011111000 30F8
*/

uint16_t cursor_white_mask[16] = {0x3000, 0x3800, 0x3C00, 0x3E00, 0x3F00, 0x3F80, 0x3FC0, 0x3FE0, 0x3FF0, 0x3FF8, 0x3FFC, 0x3FFC, 0x3FC0, 0x3DE0, 0x39F0, 0x30F8};
uint16_t cursor_black_mask[16] = {0x3000, 0x2800, 0x2400, 0x2200, 0x2100, 0x2080, 0x2040, 0x2020, 0x2010, 0x2008, 0x2004, 0x207C, 0x2340, 0x2D20, 0x2910, 0x30F8};

// The number of pixels the cursor is offset from the left of the bitmap (literally :))
#define CURSOR_LEFT_OFFSET 2

//TODO: Scale cursor
void draw_cursor(uint32_t x, uint32_t y/*, uint32_t scale*/) {
    uint32_t cursor_x = x - CURSOR_LEFT_OFFSET;
    uint32_t cursor_y = y;

    for (uint32_t i = 0; i < 16; i++) {
        uint16_t mask = cursor_white_mask[i];
        for (uint32_t j = 0; j < 16; j++) {
            if (mask & 0x8000) {
                put_pixel_global(cursor_x + j, cursor_y + i, 0xFFFFFF);
            }
            mask <<= 1;
        }
    }

    for (uint32_t i = 0; i < 16; i++) {
        uint16_t mask = cursor_black_mask[i];
        for (uint32_t j = 0; j < 16; j++) {
            if (mask & 0x8000) {
                put_pixel_global(cursor_x + j, cursor_y + i, 0x000000);
            }
            mask <<= 1;
        }
    }
}
