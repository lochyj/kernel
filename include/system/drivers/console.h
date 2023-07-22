#pragma once

#include "system/misc/ports.h"

#include <string.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define CONSOLE_DEFAULT_COLOR 0x0F

// Define simple helper functions for working with the console cursor

// Get the position of the console cursor from its index
#define CURSOR_POS_TO_INDEX(x, y) (y * VGA_WIDTH + x)
// Get the index of the console cursor from its position
#define CURSOR_INDEX_TO_POS(index, x, y) x = index % VGA_WIDTH; y = index / VGA_WIDTH;

void put_char(char character);

void initialise_console();

void clear_console();