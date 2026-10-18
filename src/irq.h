// src/irq.h

#ifndef IRQ_H
#define IRQ_H

#include "isr.h"

void irq_register(int irq, void (*handler)(struct registers *regs));

void irq_handler(struct registers *regs);

extern unsigned int irq_stub_table[16];

#endif
