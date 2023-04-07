#include "system/multiboot.h"

typedef struct multiboot_header multiboot_header_t;

void kmain(multiboot_header_t* mboot) {
    char* buffer = (char*)0xB8000;

    buffer[0] = 'E';
    buffer[2] = 'Z';


    for(;;);
}