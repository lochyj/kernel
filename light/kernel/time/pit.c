#include "light/time/pit.h"

uint32_t tick = 0;

static void timer_callback(registers_t *regs) {
    tick++;
    printf("%d\r", tick);
}

void register_PIT() {
    // Firstly, register our timer callback.
    register_interrupt_handler(IRQ0, &timer_callback);
}

void begin_timer(uint32_t frequency) {

    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    uint32_t divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);

    // Send the frequency divisor.
    outb(0x40, l);
    outb(0x40, h);
}