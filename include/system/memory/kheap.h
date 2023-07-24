#pragma once

#include <stdint.h>
#include <stddef.h>
#include "system/types.h"

// The kernel's dumb version of malloc
uintptr_t kmalloc(size_t size);

// The kernel's dumb version of malloc, but aligned
uintptr_t kmalloc_align(size_t size);

// The kernel's dumb version of malloc, but with a physical address
uintptr_t kmalloc_phys(size_t size, uintptr_t *phys);

// The kernel's dumb version of malloc, but aligned and with a physical address
uintptr_t kmalloc_align_phys(size_t size, uintptr_t *phys);
