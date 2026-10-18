// src/isr.c
// processor exception handler

#include "vga.h"
#include "string.h"
#include "isr.h"


static const char *exception_names[32] = {
    "divide by zero",
    "debug",
    "non-maskable interrupt",
    "breakpoint",
    "overflow",
    "bound range exceeded",
    "invalid opcode",
    "device is not available",
    "double fault",
    "coprocessor segment overrun",
    "invalid TSS",
    "segment not present",
    "stack-segment fault",
    "general protection fault",
    "page fault",
    "reserved",
    "x87 floating-point error",
    "alignment check",
    "machine check",
    "SIMD floating-point error",
    "virtualization error",
    "control protection error",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "hypervisor injection error",
    "VMM communication error",
    "security exception",
    "reserved"
};


// called from isr0 placeholder (isr.asm)
void isr_handler(struct registers *regs)
{
    char buffer[11];

    vga_print("Exception ");
    uint_to_string(regs->int_no, buffer);
    vga_print(buffer);
    vga_print(": ");
    if (regs->int_no < 32) {
        vga_print(exception_names[regs->int_no]);
    } else {
        vga_print("unknown");
    }
    vga_print("\n");

    vga_print("Error code: ");
    uint_to_string(regs->err_code, buffer);
    vga_print(buffer);
    vga_print("\n");

    vga_print("System halted.\n");

    // no return: `iret` would repeat the same `div` instruction
    while(1) {
        __asm__ volatile ("cli");
        __asm__ volatile ("hlt");
    }
}
