#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "system/sys/isr.h"

#define KEY_RELEASE_OFFSET 0x80

void keyboard_handler(registers_t *r);
void keyboard_install();