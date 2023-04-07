#pragma once

#define _STI() __asm__ __volatile__("sti")
#define _CLI() __asm__ __volatile__("cli")
#define _HLT() __asm__ __volatile__("hlt")
