#include "system/cpu/interrupts.h"

isr_t       interrupt_handlers[256];
idt_entry_t idt_entries[256];
idtr_t      idt_ptr;

// https://css.csail.mit.edu/6.858/2019/readings/i386.pdf - Chapter 9.1
char* exceptions_0_to_16[] = {
    "Divide error",                             // 0
    "Debug exception",                          // 1
    "Nonmaskable interrupt",                    // 2
    "Breakpoint (one-byte INT 3 instruction)",  // 3
    "Overflow (INTO instruction)",              // 4
    "Bounds check (BOUND instruction)",         // 5
    "Invalid opcode",                           // 6
    "Coprocessor not available",                // 7
    "Double fault",                             // 8
    "(reserved)",                               // 9
    "Invalid TSS",                              // 10
    "Segment not present",                      // 11
    "Stack exception",                          // 12
    "General protection",                       // 13
    "Page fault",                               // 14
    "(reserved)",                               // 15
    "Coprocessor error",                        // 16
};

/*
 * 17 - 31 are reserved by Intel
 * 32 - 47 are IRQs
 * 48 - 255 are user defined / external interrupts
 */

// Register an interrupt handler function for a given interrupt.
void register_interrupt_handler(uint8_t vector, isr_t handler) {
    IRQ_clear_mask(vector);
    interrupt_handlers[vector] = handler;
}

// Handles interrupts from the PIC. Called from isr_common_stub in interrupts.asm
void isr_handler(registers_t *regs) {

    if (interrupt_handlers[regs->int_no] != 0) {

        isr_t handler = interrupt_handlers[regs->int_no];
        handler(regs);

    } else {

        // This will never be called when we implement the masking of the IRQ in the near future.
        if (regs->int_no < 17)
            printf("Unhandled interrupt: %s\n", exceptions_0_to_16[regs->int_no]);

        else if (regs->int_no < 32)
            printf("Unhandled reserved interrupt\n");

        else
            printf("Unhandled external interrupt: IRQ %d\n", regs->int_no);

    }

}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t *regs) {
    /* Handle the interrupt in a more modular way */
    if (interrupt_handlers[regs->int_no] != 0) {
        isr_t handler = interrupt_handlers[regs->int_no];
        handler(regs);
    }

    // EOI
    if (regs->int_no >= 40) {
        outb(0xA0, 0x20); /* follower */
    }
    outb(0x20, 0x20); /* leader */

}

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;

    // Uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags   = flags /* | 0x60 */;

}

void idt_init() {
    idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

    // Remap the irq table.
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    idt_set_gate( 0, (uint32_t)isr0 , 0x08, 0x8E);
    idt_set_gate( 1, (uint32_t)isr1 , 0x08, 0x8E);
    idt_set_gate( 2, (uint32_t)isr2 , 0x08, 0x8E);
    idt_set_gate( 3, (uint32_t)isr3 , 0x08, 0x8E);
    idt_set_gate( 4, (uint32_t)isr4 , 0x08, 0x8E);
    idt_set_gate( 5, (uint32_t)isr5 , 0x08, 0x8E);
    idt_set_gate( 6, (uint32_t)isr6 , 0x08, 0x8E);
    idt_set_gate( 7, (uint32_t)isr7 , 0x08, 0x8E);
    idt_set_gate( 8, (uint32_t)isr8 , 0x08, 0x8E);
    idt_set_gate( 9, (uint32_t)isr9 , 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

    idt_set_gate(32, (uint32_t)irq0 , 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1 , 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2 , 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3 , 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4 , 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5 , 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6 , 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7 , 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8 , 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9 , 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);

    IRQ_set_all_mask();
    IRQ_clear_mask(IRQ2);

    idt_flush((uint32_t)&idt_ptr);
}

//TODO: mask all of the IRQs before enabling them (except IRQ2)

void IRQ_set_all_mask() {
    for (int8_t i = 0; i < 16; i++){
        IRQ_set_mask(i);
    }
}

void IRQ_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);
}

void IRQ_clear_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);
}
