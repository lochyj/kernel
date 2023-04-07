#include "system/multiboot.h"
#include "drivers/display.h"

typedef struct multiboot_header multiboot_header_t;

void kmain(multiboot_header_t* mboot) {
    clear_screen();

    puts("Hello there!");


    for(;;);
}