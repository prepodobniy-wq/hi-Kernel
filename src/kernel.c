// src/kernel.c

#include "vga.h"
#include "string.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "pic.h"
#include "irq.h"
#include "timer.h"
#include "keyboard.h"


void kernel_main(void)
{
    int i;
    char c;

    vga_clear();
    vga_print("hi!Kernel started\n");
    
    vga_print("[ INIT ] Loading GDT...\n");
    gdt_init();
    vga_print("[ OK ] GDT loaded.\n");

    vga_print("[ INIT ] Loading IDT...\n");
    idt_init();
    for (i=0; i < 32; i++) {
        idt_set_gate(i, isr_stub_table[i]);
    }
    vga_print("[ OK ] IDT loaded.\n");

    vga_print("[ INIT ] Loading PIC...\n");
    pic_init();
    for (i = 0; i < 16; i++) {
        idt_set_gate(32 + i, irq_stub_table[i]);
    }
    vga_print("[ OK ] PIC loaded.\n");

    timer_init(100);                 // 100 ticks per second
    pic_unmask(0);
    vga_print("[ OK ] Timer started.\n");

    keyboard_init();
    pic_unmask(1);

    __asm__ volatile ("sti");       // turning on interrupts

    while (1) {
        c = keyboard_get_char();
        if (c != 0) {
            vga_put_char(c);
        } else {
            __asm__ volatile ("hlt");
        }
    }
}
