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
#include "shell.h"
#include "multiboot.h"
#include "pmm.h"


void kernel_main(unsigned int magic, unsigned int info_addr)
{
    int i;
    char c;
    char buffer[11];
    unsigned int *info = (unsigned int *)info_addr;
    unsigned int mmap_addr;
    unsigned int mmap_end;
    unsigned int page_a, page_b, page_c;
    struct mmap_entry *entry;

    vga_clear();
    vga_print("hi!Kernel started\n");
    
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        vga_print("[ WARN ] Not loaded by a Multiboot loader\n");
    } else if ((info[0] & 1) == 0) {
        vga_print("Memory info: not provided");
    } else {
        vga_print("Memory below 1MB: ");
        uint_to_string(info[1], buffer);
        vga_print(buffer);
        vga_print(" KB\n");

        vga_print("Memory above 1MB: ");
        uint_to_string(info[2], buffer);
        vga_print(buffer);
        vga_print(" KB\n");

        if (info[0] & MULTIBOOT_FLAG_MMAP) {
            mmap_addr = info[12];
            mmap_end = info[12] + info[11];
            vga_print("Memory map:\n");
            while (mmap_addr < mmap_end) {
                entry = (struct mmap_entry *)mmap_addr;
                vga_print("  ");
                uint_to_hex(entry->base_low, buffer);
                vga_print(buffer);
                vga_print(" len ");
                uint_to_hex(entry->length_low, buffer);
                vga_print(buffer);
                vga_print(" type ");
                uint_to_string(entry->type, buffer);
                vga_print(buffer);
                vga_print("\n");
                mmap_addr = mmap_addr + entry->size + 4;
            }
        }
    }

    pmm_init(info);
    vga_print("Free pages: ");
    uint_to_string(pmm_free_count(), buffer);
    vga_print(buffer);
    vga_print("\n");

    page_a = pmm_alloc_page();
    page_b = pmm_alloc_page();
    pmm_free_page(page_a);
    page_c = pmm_alloc_page();
    vga_print("Pages a, b, c: ");
    uint_to_hex(page_a, buffer);
    vga_print(buffer);
    vga_print(" ");
    uint_to_hex(page_b, buffer);
    vga_print(buffer);
    vga_print(" ");
    uint_to_hex(page_c, buffer);
    vga_print(buffer);
    vga_print("\n");
    pmm_free_page(page_b);
    pmm_free_page(page_c);

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
    sh_init();

    __asm__ volatile ("sti");       // turning on interrupts

    while (1) {
        c = keyboard_get_char();
        if (c != 0) {
            sh_handle_char(c);
        } else {
            __asm__ volatile ("hlt");
        }
    }
}
