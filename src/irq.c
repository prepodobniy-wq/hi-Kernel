// src/irq.c

#include "isr.h"
#include "ports.h"
#include "irq.h"

#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC_EOI      0x20

static void (*irq_handlers[16])(struct registers *regs);

void irq_register(int irq, void (*handler)(struct registers *regs))
{
    if (irq >= 0 && irq < 16) {
        irq_handlers[irq] = handler;
    }
}

void irq_handler(struct registers *regs)
{
    int irq = (int)regs->int_no - 32;

    if (irq >= 0 && irq < 16 && irq_handlers[irq] != 0) {
        irq_handlers[irq](regs);
    }

    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}
