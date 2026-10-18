// src/idt.c
// Interrupt Descriptor Table

// one IDT entry (8 bytes)
struct idt_entry {
    unsigned short offset_low;     // handler address, bits 0-15
    unsigned short selector;       // code segment selector
    unsigned char  zero;           // always 0
    unsigned char  type_attr;      // gateway type and permissions
    unsigned short offset_high;    // handler address, bits 16-31
} __attribute__((packed));

// pointer to IDT for the lidt command
struct idt_pointer {
    unsigned short limit;        // table size in bytes - 1
    unsigned int   base;         // table address
} __attribute__((packed));

#define IDT_ENTRIES 256

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_pointer idt_ptr;

// written in `idt_load.asm` file
extern void idt_load(unsigned int pointer_address);

// writes the handler for the interrupt with number `index` to the table
void idt_set_gate(int index, unsigned int handler_address)
{
    idt[index].offset_low   = handler_address & 0xFFFF;
    idt[index].selector     = 0x08;
    idt[index].zero         = 0;
    idt[index].type_attr    = 0x8E;
    idt[index].offset_high  = (handler_address >> 16) & 0xFFFF;
}

void idt_init(void)
{
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base  = (unsigned int)&idt;

    idt_load((unsigned int)&idt_ptr);
}
