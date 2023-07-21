#include <stdio.h>

#include "system/gdt.h"

const char* KERNEL_VERSION = "v0.1.0";
const char* USER = "Lochyj";

void kmain() {
	initialise_console();

	gdt_install();
	printf("Loaded the GDT successfully!\n");
	


	printf(" ____  _ _       _     ____   _____\n");
    printf("|  _ \\| (_)     | |   / __ \\ / ____|\n");
    printf("| |_) | |_ _ __ | | _| |  | | (___\n");
    printf("|  _ <| | | '_ \\| |/ / |  | |\\___ \\ \n");
    printf("| |_) | | | | | |   <| |__| |____) |\n");
    printf("|____/|_|_|_| |_|_|\\_\\\\____/|_____/\n");
    printf("Kernel version %s; User: %s\n", KERNEL_VERSION, USER);
    printf("console@%s> ", USER);

}