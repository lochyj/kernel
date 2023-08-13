#include "light/vm/qemu.h"

// Taken from https://github.com/klange/toaruos/blob/master/boot/qemu.c and modified
void detect_qemu(void) {
    /* Try to detect qemu headless boot */
    outb(0x510, 0x0000);

	if (inb(0x511) == 'Q' && inb(0x511) == 'E' && inb(0x511) == 'M' && inb(0x511) == 'U') {
       printf("QEMU detected!\n");
    }
}