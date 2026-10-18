// src/pic.c
// interrupt controller setup (8259 PIC)

#include "ports.h"
#include "pic.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

void pic_init(void)
{
    // ICW1
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);

    // ICW2
    outb(PIC1_DATA, 32);
    outb(PIC2_DATA, 40);

    // ICW3
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    // ICW4
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

void pic_unmask(int irq)
{
    unsigned short port;
    unsigned char mask;

    if (irq < 0 || irq >= 16) {
        return;
    }

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        pic_unmask(2);
        port = PIC2_DATA;
        irq = irq - 8;
    }

    mask = inb(port);
    mask = (unsigned char)(mask & ~(1 << irq));
    outb(port, mask);
}
