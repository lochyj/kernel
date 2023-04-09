#include "system/multiboot.h"
#include "system/sys/gdt.h"
#include "system/assemblies.h"
#include <stdio.h>

typedef struct multiboot_header multiboot_header_t;

void kmain(multiboot_header_t* mboot) {

    // If not using GRUB, comment out the following line.
    if (!(mboot->magic & MULTIBOOT_HEADER_MAGIC)) {
        // Something went terribly wrong in the boot stage.
        for(;;);
    }
    init_video();

    printf("GDT Initialisation: ");
    gdt_init();
    printf("Complete.\n");

    printf("test11");

    // Infinite loop until I figure out why the loop in boot.asm isn't working.
    for(;;) NOP();
}