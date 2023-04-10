#include "system/multiboot.h"
#include "system/assemblies.h"

#include "system/sys/gdt.h"
#include "system/sys/idt.h"
#include "system/sys/isr.h"
#include "system/sys/irq.h"

#include "drivers/keyboard.h"
#include "drivers/display.h"

#include <stdio.h>

typedef struct multiboot_header multiboot_header_t;

void kmain(multiboot_header_t* mboot) {

    // If not using GRUB, comment out the following line.
    if (!(mboot->magic & MULTIBOOT_HEADER_MAGIC)) {
        // Something went terribly wrong in the boot stage.
        for(;;);
    }
    init_video();

    CLI();

    printf("Kernel Initialisation:\n");
    gdt_init();
    idt_install();
    isr_install();
    irq_install();

    STI();
    
    // keyboard_install();

    // Infinite loop until I figure out why the loop in boot.asm isn't working.
    for(;;) NOP();
}