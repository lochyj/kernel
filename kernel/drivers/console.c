#include "system/drivers/console.h"

//TODO: cleanup this mess...
//TODO: implement scrolling

volatile char* video_memory = (volatile char*)0xB8000;

// From: https://wiki.osdev.org/Text_Mode_Cursor
void enable_console_cursor(uint8_t cursor_start, uint8_t cursor_end) {
    outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

// From: https://wiki.osdev.org/Text_Mode_Cursor
void update_console_cursor(int x, int y) {
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

// From: https://wiki.osdev.org/Text_Mode_Cursor
uint16_t get_console_cursor() {
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

void clear_console() {
    unsigned short blank = 0x20 | (CONSOLE_DEFAULT_COLOR << 8);

    // Set the console to be blank spaces with the default color
    for(int i = 0; i < VGA_HEIGHT; i++)
        // the first param actually is a pointer its just gcc doesn't realize
        memsetw((unsigned short)(video_memory + i * VGA_WIDTH), blank, VGA_WIDTH);

    update_console_cursor(0, 0);
}

void put_char(char character) {
    uint16_t cursor = get_console_cursor();
    int x, y;
    CURSOR_INDEX_TO_POS(cursor, x, y);

    if (character == '\n') {
        update_console_cursor(0, y + 1);
        return;
    } else if (character == '\r') {
        update_console_cursor(0, y);
        return;
    } else if (character == '\b') {
        if (x == 0 && y == 0)
            return;

        if (x == 0) {
            update_console_cursor(VGA_WIDTH - 1, y - 1);
        } else {
            update_console_cursor(x - 1, y);
        }
        put_char(' ');
        update_console_cursor(x - 1, y);
        return;
    } else if (character == 0x00) {
        return;
    }

    video_memory[cursor * 2] = character | (CONSOLE_DEFAULT_COLOR << 8);
    update_console_cursor(x + 1, y);
}

void initialise_console() {
    clear_console();
    enable_console_cursor(0, 15);
}
