#include "system/memory/kheap.h"

// End is defined in the linker script
extern void* end;
uintptr_t placement_pointer = (uintptr_t)&end;

uintptr_t kmalloc_internal(size_t size, int align, uintptr_t * phys) {
	if (align && (placement_pointer & 0xFFFFF000)) {
		placement_pointer &= 0xFFFFF000;
		placement_pointer += 0x1000;
	}

	if (phys) {
		*phys = placement_pointer;
	}

	uintptr_t address = placement_pointer;

	placement_pointer += size;

	return (uintptr_t)address;
}

uintptr_t kmalloc(size_t size) {
	return kmalloc_internal(size, 0, NULL);
}

uintptr_t kmalloc_align(size_t size) {
    return kmalloc_internal(size, 1, NULL);
}

uintptr_t kmalloc_phys(size_t size, uintptr_t *phys) {
    return kmalloc_internal(size, 0, phys);
}

uintptr_t kmalloc_align_phys(size_t size, uintptr_t *phys) {
    return kmalloc_internal(size, 1, phys);
}
