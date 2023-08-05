#include "system/debug/debug.h"

/**
    Source: https://wiki.osdev.org/Printing_To_Screen
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
int __DEBUG_print(const char* string, int offset) {
    volatile char *video = (volatile char*)0xB8000;
    video += offset;
    while( *string != 0 )
    {
        *video++ = *string++;
        *video++ = 0x4F;    // white text on a red background
        offset += 2;
    }
    return offset;
}


// Created by Robert Jan Schaper: https://groups.google.com/g/comp.lang.c++.moderated/c/ihafayWmWU8/m/zNFNsvzIxAAJ
char* __DEBUG_itoa(int val, int base) {
	
	static char buf[32] = {0};
	
	int i = 30;
	
	for (; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];

}

void __DEBUG_PANIC(const char* file, int line, const char* message) {

    int offset = __DEBUG_print("KERNEL PANIC | At File: ", 0);

    offset = __DEBUG_print(file, offset);
    offset = __DEBUG_print(" | On Line: ", offset);
    offset = __DEBUG_print(__DEBUG_itoa(line, 10), offset);
    offset = __DEBUG_print(" | Error: ", offset);

    __DEBUG_print(message, offset);
    __DEBUG_halt();

}

void __DEBUG_ERROR(const char* file, int line) {

    int offset = __DEBUG_print("ASSERTION FAILED | At File: ", 0);

    offset = __DEBUG_print(file, offset);
    offset = __DEBUG_print(" | On Line: ", offset);

    __DEBUG_print(__DEBUG_itoa(line, 10), offset);
    __DEBUG_halt();

}

void __DEBUG_halt() {
    asm volatile("cli; hlt");
}
