/* This file contains the implementation of a PS/2 mouse driver that has been derived and modified from:
 * https://github.com/szhou42/osdev/blob/master/src/kernel/drivers/mouse.c
*/

#include "system/drivers/mouse.h"

uint8_t mouse_cycle = 0;
int8_t  mouse_byte[3];
mouse_data_t mouse_data;

#define PACKETS_IN_PIPE 1024
#define DISCARD_POINT 32

#define MOUSE_IRQ 12

#define MOUSE_PORT   0x60
#define MOUSE_STATUS 0x64
#define MOUSE_ABIT   0x02
#define MOUSE_BBIT   0x01
#define MOUSE_WRITE  0xD4
#define MOUSE_F_BIT  0x20
#define MOUSE_V_BIT  0x08

/*
 * Every time mouse event fires, mouse_handler will be called
 * The argument regs is not used in here
 * */
void mouse_handler(isr_t* r) {
	uint8_t status = inb(MOUSE_STATUS);
	while (status & MOUSE_BBIT) {
		int8_t mouse_in = inb(MOUSE_PORT);
		if (status & MOUSE_F_BIT) {
			switch (mouse_cycle) {
				case 0:
					mouse_byte[0] = mouse_in;
					if (!(mouse_in & MOUSE_V_BIT)) return;
					++mouse_cycle;
					break;
				case 1:
					mouse_byte[1] = mouse_in;
					++mouse_cycle;
					break;
				case 2:
					mouse_byte[2] = mouse_in;
					/* We now have a full mouse packet ready to use */
					if (mouse_byte[0] & 0x80 || mouse_byte[0] & 0x40) {
						/* x/y overflow? bad packet! */
                        PANIC("Mouse overflow | Received bad packet");
						break;
					}
					mouse_device_packet_t packet;
					packet.magic = MOUSE_MAGIC;
					packet.x_difference = mouse_byte[1];
					packet.y_difference = mouse_byte[2];
					packet.buttons = 0;
					if (mouse_byte[0] & 0x01) {
						packet.buttons |= LEFT_CLICK;
					}
					if (mouse_byte[0] & 0x02) {
						packet.buttons |= RIGHT_CLICK;
					}
					if (mouse_byte[0] & 0x04) {
						packet.buttons |= MIDDLE_CLICK;
					}
					mouse_cycle = 0;

                    mouse_data.x += packet.x_difference;
                    mouse_data.y -= packet.y_difference;
                    set_mouse_buttons(&packet, &mouse_data);

                    if (mouse_data.log_mouse_data) {
                        print_mouse_data(&mouse_data);
                    }

					break;
			}
		}
		status = inb(MOUSE_STATUS);
	}
}

void print_mouse_data(mouse_data_t* mouse_data) {
    printf("                                                                              \r");
    printf("Mouse: x: %d; y: %d; left: %d; right: %d; middle: %d;\r", mouse_data->x, mouse_data->y, mouse_data->left_button_down, mouse_data->right_button_down, mouse_data->middle_button_down);
}

void set_mouse_logging(bool log_mouse_data) {
    mouse_data.log_mouse_data = log_mouse_data;
}

void set_mouse_buttons(mouse_device_packet_t* packet, mouse_data_t* mouse_data) {
    if (packet->buttons == 0) {
        mouse_data->left_button_down = false;
        mouse_data->right_button_down = false;
        mouse_data->middle_button_down = false;
    } else if(packet->buttons == 1) {
        mouse_data->left_button_down = true;

        mouse_data->right_button_down = false;
        mouse_data->middle_button_down = false;
    } else if(packet->buttons == 2) {
        mouse_data->left_button_down = false;

        mouse_data->right_button_down = true;

        mouse_data->middle_button_down = false;
    } else if(packet->buttons == 3) {
        mouse_data->left_button_down = true;
        mouse_data->right_button_down = true;

        mouse_data->middle_button_down = false;
    } else if(packet->buttons == 4) {
        mouse_data->left_button_down = false;
        mouse_data->right_button_down = false;

        mouse_data->middle_button_down = true;
    } else if(packet->buttons == 5) {
        mouse_data->left_button_down = true;

        mouse_data->right_button_down = false;

        mouse_data->middle_button_down = true;
    } else if(packet->buttons == 6) {
        mouse_data->left_button_down = false;

        mouse_data->right_button_down = true;
        mouse_data->middle_button_down = true;
    } else if(packet->buttons == 7) {
        mouse_data->left_button_down = true;
        mouse_data->right_button_down = true;
        mouse_data->middle_button_down = true;
    }
}

void mouse_write(uint8_t a_write) {

    //Tell the mouse we are sending a command
    mouse_wait(1);

    outb(0x64, 0xD4);

    mouse_wait(1);

    //Finally write
    outb(0x60, a_write);
}

uint8_t mouse_read() {
    mouse_wait(0);
    return inb(0x60);
}

void mouse_wait(uint8_t a_type) {
    uint32_t _time_out = 100000;
    if (a_type == 0) {
        while(_time_out--) {
            if((inb(0x64) & 1)==1)
            {
                return;
            }
        }
        return;
    }
    else {
        while (_time_out--) {
            if((inb(0x64) & 2)==0) {
                return;
            }
        }
        return;
    }
}

/*
 * Mouse initialization, register irq and other stuff
 * */
void init_mouse() {

    mouse_data.log_mouse_data = false;

    mouse_data.x = 0;
    mouse_data.y = 0;
    mouse_data.left_button_down = false;
    mouse_data.right_button_down = false;
    mouse_data.middle_button_down = false;

    uint8_t _status;

    //Enable the auxiliary mouse device
    mouse_wait(1);
    outb(0x64, 0xA8);

    //Enable the interrupts
    mouse_wait(1);
    outb(0x64, 0x20);
    mouse_wait(0);
    _status = (inb(0x60) | 2);
    mouse_wait(1);
    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, _status);

    // Tell the mouse to use default settings
    mouse_write(0xF6);
    mouse_read();  //Acknowledge

    // Enable the mouse
    mouse_write(0xF4);
    mouse_read();  //Acknowledge

    // Setup the mouse handler
    register_interrupt_handler(0x20 + 12, mouse_handler);
}