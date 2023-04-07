#pragma once

#include <stdint.h>
#include "misc/ports.h"

#define VIDEO_ADDRESS   0xB8000 //* Video memory starts here.
#define VGA_WIDTH       80      //* 80 x Characters


void        set_cursor(uint32_t x, uint32_t y);
uint16_t    get_cursor();
void        enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void        set_char_at_video_memory(char character, uint16_t offset);
uint16_t    get_offset(uint32_t x, uint32_t y);
void        clear_screen();
