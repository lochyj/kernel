#pragma once

#include <stdint.h>

#define VIDEO_ADDRESS   0xB8000 //* Video memory starts here.
#define VGA_WIDTH       80      //* 80 x Characters
#define VGA_HEIGHT      25      //* 25 y Characters
#define VGA_COLOUR      0xF0    //* White on Black

void scroll(void);
void move_csr(void);
void clear_screen();
void putc(char c);
void puts(char* text);
void settextcolor(unsigned char forecolor, unsigned char backcolor);
void init_video(void);
