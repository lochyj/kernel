#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "system/drivers/pic.h"
#include "system/cpu/interrupts.h"
#include "system/debug/debug.h"

typedef enum {
	LEFT_CLICK   = 0x01,
	RIGHT_CLICK  = 0x02,
	MIDDLE_CLICK = 0x04
} mouse_click_t;

typedef struct {
	uint32_t magic;
	int8_t x_difference;
	int8_t y_difference;
	mouse_click_t buttons;
} mouse_device_packet_t;

typedef struct {
    int32_t x;
    int32_t y;
    bool left_button_down;
    bool right_button_down;
    bool middle_button_down;

    bool log_mouse_data;
} mouse_data_t;

#define MOUSE_MAGIC 0xFEED1234

void print_mouse_state();

void set_mouse_logging(bool log_mouse_data);

int left_button_down();
int right_button_down();

int left_button_up();
int right_button_up();

void mouse_wait(uint8_t a_type);

void init_mouse();

void mouse_handler(isr_t* regs);

void mouse_write(uint8_t a_write);
uint8_t mouse_read();
