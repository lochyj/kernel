#include "drivers/display.h"

// The use of unsigned int (uint32_t) is to prevent the offset of the cursor being negative and causing a crash.

void set_cursor(uint32_t x, uint32_t y) {
	uint16_t pos = y * VGA_WIDTH + x;

	port_byte_out(0x3D4, 0x0F);
	port_byte_out(0x3D5, (uint8_t) (pos & 0xFF));
	port_byte_out(0x3D4, 0x0E);
	port_byte_out(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

uint16_t get_cursor() {
    uint16_t position = 0;
    port_byte_out(0x3D4, 0x0F);
    position |= port_byte_in(0x3D5);
    port_byte_out(0x3D4, 0x0E);
    position |= ((uint16_t)port_byte_in(0x3D5)) << 8;
    return position;
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
	port_byte_out(0x3D4, 0x0A);
	port_byte_out(0x3D5, (port_byte_in(0x3D5) & 0xC0) | cursor_start);
 
	port_byte_out(0x3D4, 0x0B);
	port_byte_out(0x3D5, (port_byte_in(0x3D5) & 0xE0) | cursor_end);
}

void set_char_at_video_memory(char character, uint16_t offset) {
    unsigned char* videoMemory  = (unsigned char*) 0xB8000;
    videoMemory[offset]         = character;
    videoMemory[offset + 1]     = video_color;
}

uint16_t get_offset(uint32_t x, uint32_t y) {
    return (uint16_t)(x * VGA_WIDTH + y);
}

void clear_screen() {
    uint32_t screen_size = MAX_COLS * MAX_ROWS;
    for (uint32_t i = 0; i < screen_size; ++i) {
        set_char_at_video_memory(' ', i * 2);
    }
    set_cursor(0, 0);
}


