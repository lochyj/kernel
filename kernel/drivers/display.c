#include "drivers/display.h"
#include "misc/ports.h"
#include <string.h>

// The use of unsigned int (uint32_t) is to prevent the offset of the cursor being negative and causing a crash.

uint32_t x = 0, y = 0;

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void disable_cursor() {
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

static void update_cursor(uint32_t x, uint32_t y) {
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

static uint16_t get_cursor_position(void) {
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos          |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos          |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

static uint16_t get_offset(int x, int y) {
    return (uint16_t)(x * VGA_WIDTH + y);
}

static uint16_t scroll_ln(int x, int y) {
    memcpy(
        (uint8_t*) (get_offset(0, 1) + 0xB8000),
        (uint8_t*) (get_offset(0, 0) + 0xB8000),
        VGA_WIDTH * (VGA_HEIGHT - 1) * 2
    );

    for (int col = 0; col < VGA_WIDTH; col++) {
        unsigned char* videoMemory = (unsigned char *) VIDEO_ADDRESS;
        videoMemory[get_offset(col, VGA_HEIGHT - 1)] = ' ';
        videoMemory[get_offset(col, VGA_HEIGHT - 1) + 1] = VGA_COLOUR;
    }

    return get_offset(x, y) - 2 * VGA_WIDTH;
}

void clear_screen() {
    int screen_size = VGA_WIDTH * VGA_HEIGHT;
    for (int i = 0; i < screen_size; ++i) {
        unsigned char* videoMemory = (unsigned char *) VIDEO_ADDRESS;
        videoMemory[i * 2] = ' ';
        videoMemory[i * 2 + 1] = VGA_COLOUR;
    }
    x, y = 0;
    update_cursor(0, 0);
}

/*  Source: https://wiki.osdev.org/Printing_To_Screen
    VGA Text Mode Colour Table
    Color number 	Color name
    0 	            Black
    1 	            Blue
    2 	            Green
    3 	            Cyan
    4 	            Red
    5 	            Purple
    6 	            Brown
    7 	            Gray
    8 	            Dark Gray
    9 	            Light Blue
    10 || A 	    Light Green
    11 || B	        Light Cyan
    12 || C	        Light Red
    13 || D	        Light Purple
    14 || E	        Yellow
    15 || F	        White

    VGA Colour Code: 0xYZ
    Where Y is the background color
    And Z is the text color
    EG: 
        0x4F is white text on a red background,
        0x0F is white text on a black background.
*/

void putc(char c) {
	if(c & (char)0x00) return;
	
	if(c & (char)'\n') {
		y++;
		x = 0;
        update_cursor(x, y);
		return;
	}

    if(c & (char)'\r') {
		x = 0;
        update_cursor(x, y);
		return;
	}

	if (x >= VGA_WIDTH) {
        if (y >= VGA_HEIGHT) {
            y = VGA_HEIGHT;
            x = 0;
            scroll_ln(x, y);
        } else {
            x = 0;
            y++;
        }
	}

    volatile char* display = (volatile char*) VIDEO_ADDRESS;

	display[(y * VGA_WIDTH + x) * 2] = c;
	display[(y * VGA_WIDTH + x) * 2 + 1] = VGA_COLOUR;
	x++;
	update_cursor(x, y);
}

void puts(const char* string) {
    while( *string != 0 ) {
		putc(*string++);
	}
}