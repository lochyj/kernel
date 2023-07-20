#include <stdio.h>

#include "system/gdt.h"

void kmain() {
	initialise_console();

	gdt_install();
	printf("Loaded the GDT successfully!\n");
	


	printf("Hello, kernel World!\n");

}