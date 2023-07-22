#include <stdio.h>

#include "system/memory/gdt.h"
#include "system/cpu/interrupts.h"
#include "system/drivers/pit.h"

const char* KERNEL_VERSION = "v0.1.0";
const char* USER = "lochyj";

void kmain() {
   // I think its ok to put this here? Its useful anyways.
	initialise_console();

	gdt_install();
	printf("Loaded the GDT successfully!\n");

	idt_init();
	printf("Loaded the IDT and ISR successfully!\n");

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

   init_timer(50);

}