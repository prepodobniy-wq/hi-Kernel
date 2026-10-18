// src/idt.h
// setting up IDT

#ifndef IDT_H
#define IDT_H

void idt_init(void);
void idt_set_gate(int index, unsigned int handler_address);

#endif
