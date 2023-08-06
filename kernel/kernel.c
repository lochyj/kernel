#include <stdio.h>
#include <stdint.h>

#include "system/memory/gdt.h"
#include "system/cpu/interrupts.h"
#include "system/drivers/pit.h"
#include "system/drivers/keyboard.h"
#include "system/drivers/mouse.h"
#include "system/misc/multiboot.h"
#include "system/memory/mem.h"
#include "system/gui/core.h"

#include "system/debug/debug.h"

const char* KERNEL_VERSION = "v0.1.1";
const char* USER = "lochyj";

extern void call_function_from_pointer(uintptr_t);

void kmain(multiboot_info_t* multiboot_header, uint32_t multiboot_magic) {

   // Check if the bootloader is multiboot compliant
   if (multiboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
      PANIC("Bootloader is not multiboot compliant!");
   }

   // Text mode check
   if(multiboot_header->framebuffer_type == 1) {
      // It isn't a text mode.

      initialise_VBE(multiboot_header);

      for (int i = 100; i < 200; i++) {
         for (int j = 100; j < 200; j++) {
            put_pixel(i, j, 0xFFFFFF);
         }
      }

      for(;;);
	}

   uint32_t address_of_module = multiboot_header->mods_addr;


   // I think its ok to put this here? Its useful anyways.
	initialise_console();

   printf("Total Memory: %dkb of memory available.\n", multiboot_header->mem_upper + multiboot_header->mem_lower);
   printf("Location of module %d addr: 0x%x\n", multiboot_header->mods_count, address_of_module);
   printf("VBE Video mode info: %d\n", multiboot_header->vbe_mode_info);

	gdt_install();
	printf("Loaded the GDT successfully!\n");

	idt_init();
	printf("Loaded the IDT and ISR successfully!\n");

   paging_install(multiboot_header->mem_upper + multiboot_header->mem_lower);
   printf("Successfully initialized paging!\n");

   heap_install();
   printf("Successfully installed the heap!\n");

   // NOTE: you should initialize any interrupt handlers before sti
   // So register them here:
   //register_PIT();

   init_keyboard();
   printf("Initialized the PS/2 keyboard driver successfully!\n");

   init_mouse();
   printf("Initialized the PS/2 mouse driver successfully!\n");

   // For debugging and visualizing mouse position:
   set_mouse_logging(false);


   // ---

   // Enable interrupts
   asm volatile("sti");
   printf("Successfully enabled interrupts!\n");

	printf(" ____  _ _       _     ____   _____\n");
   printf("|  _ \\| (_)     | |   / __ \\ / ____|\n");
   printf("| |_) | |_ _ __ | | _| |  | | (___\n");
   printf("|  _ <| | | '_ \\| |/ / |  | |\\___ \\ \n");
   printf("| |_) | | | | | |   <| |__| |____) |\n");
   printf("|____/|_|_|_| |_|_|\\_\\\\____/|_____/\n");
   printf("Kernel version %s; User: %s\n", KERNEL_VERSION, USER);
   printf("console@%s> ", USER);
   //printf("\n");

   // Testing:
   //printf("\n\n");
   //asm volatile("int $0x5");
   //PANIC("Testing panic");
   //begin_timer(100);
   // ---

   for (;;) {
      asm volatile("nop");
   }

}