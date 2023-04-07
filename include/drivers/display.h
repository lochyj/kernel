#pragma once

#include <stdint.h>

#define VIDEO_ADDRESS   0xB8000 //* Video memory starts here.
#define VGA_WIDTH       80      //* 80 x Characters
#define VGA_HEIGHT      25      //* 25 y Characters
#define VGA_COLOUR      0x0F    //* White on Black


void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

void disable_cursor();

void putc(char c);

void puts(const char* string);
