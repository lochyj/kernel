#include <stdint.h>
#include <stdbool.h>

// TODO: replace this with the kernel's logging system
#include <stdio.h>

#include "light/mm/gdt.h"
#include "light/mm/mem.h"

#include "light/irq/interrupts.h"

#include "light/time/pit.h"

#include "drivers/display/console.h"

#include "drivers/display/core.h"
#include "drivers/display/window.h"

#include "drivers/input/keyboard.h"
#include "drivers/input/mouse.h"

#include "light/multiboot.h"
#include "light/debug.h"

#include "light/fs/fs.h"
#include "light/fs/initrd.h"

const char* KERNEL_VERSION = "v0.1.1";
const char* USER = "lochyj";

uint32_t initial_esp;

void kmain(multiboot_info_t* multiboot_header, uint32_t initial_stack, uint32_t multiboot_magic) {

	// Check if the bootloader is multiboot compliant
	if (multiboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		PANIC("Bootloader is not multiboot compliant!");
	}

	// Text mode check
	if(multiboot_header->framebuffer_type == 1) {

		for(;;);

	}

	uint32_t address_of_module = multiboot_header->mods_addr;

	// I think its ok to put this here? Its useful anyways.
	initialise_console();

	printf("          _      _       _     _		\n");
	printf("    ____ | |    (_)     | |   | |  		\n");
	printf("   / __ \\| |     _  __ _| |__ | |_ 	\n");
	printf("  / / _` | |    | |/ _` | '_ \\| __|	\n");
	printf(" | | (_| | |____| | (_| | | | | |_ 		\n");
	printf("  \\ \\__,_|______|_|\\__, |_| |_|\__|	\n");
	printf("   \\____/           __/ |				\n");
	printf("                   |___/				\n");

	// print multiboot header

	// printf("## Multiboot headers: ##\n");
	// printf("Flags: 0x%x  |  ", multiboot_header->flags); printf("Mem Lower: %dkb\n", multiboot_header->mem_lower);
	// printf("Mem Upper: %dkb |  ", multiboot_header->mem_upper); printf("Boot Device: 0x%x\n", multiboot_header->boot_device);
	// printf("Cmdline: %s  |  ", (char*)multiboot_header->cmdline); printf("Mods Count: %d\n", multiboot_header->mods_count);
	// printf("Mods Addr: 0x%x |  ", multiboot_header->mods_addr); printf("Mmap Length: %d\n", multiboot_header->mmap_length);
	// printf("Mmap Addr: 0x%x |  ", multiboot_header->mmap_addr); printf("Drives Length: %d\n", multiboot_header->drives_length);
	// printf("Drives Addr: 0x%x  |   ", multiboot_header->drives_addr); printf("Config Table: 0x%x\n", multiboot_header->config_table);
	// printf("Boot Loader Name: %s  |  ", (char*)multiboot_header->boot_loader_name); printf("APM Table: 0x%x\n", multiboot_header->apm_table);
	// printf("VBE Control Info: 0x%x   |  ", multiboot_header->vbe_control_info); printf("VBE Mode Info: 0x%x\n", multiboot_header->vbe_mode_info);
	// printf("VBE Mode: 0x%x  |  ", multiboot_header->vbe_mode); printf("VBE Interface Seg: 0x%x\n", multiboot_header->vbe_interface_seg);
	// printf("VBE Interface Off: 0x%x  |  ", multiboot_header->vbe_interface_off); printf("VBE Interface Len: 0x%x\n", multiboot_header->vbe_interface_len);

	printf("\n##Other kernel info: ##\n");
	printf("Total Memory: %dkb of memory available. |  ", multiboot_header->mem_upper + multiboot_header->mem_lower); printf("Location of module %d addr: 0x%x\n", multiboot_header->mods_count, address_of_module);

	gdt_install();
	printf("Loaded the GDT successfully!\n");

	idt_init();
	printf("Loaded the IDT and ISR successfully!\n");

	ASSERT(multiboot_header->mods_count > 0);

	uint32_t initrd_location = *((uint32_t*)multiboot_header->mods_addr);
	uint32_t initrd_end = *(uint32_t*)(multiboot_header->mods_addr + 4);

	placement_pointer = initrd_end;

	paging_install(multiboot_header->mem_upper + multiboot_header->mem_lower);
	printf("Successfully initialized paging and installed the heap!\n");

	fs_root = initialise_initrd(initrd_location);

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