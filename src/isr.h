// src/isr.h
// processor exception handler

#ifndef ISR_H
#define ISR_H

struct registers {
    unsigned int edi, esi, ebp ,esp , ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags;
};

void isr_handler(struct registers *regs);

extern unsigned int isr_stub_table[32];

#endif
