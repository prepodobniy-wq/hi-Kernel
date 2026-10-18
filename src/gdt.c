// src/gdt.c
// Global Descriptor Table (GDT) - flat model memory

// one GDT entry (8 bytes)
struct gdt_entry {
    unsigned short limit_low;    // limit, bits 0-15
    unsigned short base_low;     // base, bits 0-15
    unsigned char  base_middle;  // base, bits 16-23
    unsigned char  access;       // right access
    unsigned char  granularity;  // flags + limit, bits 16-19
    unsigned char  base_high;    // base, bits 24-31
} __attribute__((packed));

// pointer to the GDT, accepted by the `lgdt` instruction (6 bytes)
struct gdt_pointer {
    unsigned short limit;         // table size in bytes - 1
    unsigned int   base;          // table address
} __attribute__((packed));

#define GDT_ENTRIES 3

static struct gdt_entry gdt[GDT_ENTRIES];
static struct gdt_pointer gdt_ptr;

// written in `gdt_flush.asm` file
extern void gdt_flush(unsigned int pointer_address);

static void gdt_set_entry(int index, unsigned int base, unsigned int limit, unsigned char access, unsigned char flags)
{
    gdt[index].base_low    = base & 0xFFFF;
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high   = (base >> 24) & 0xFF;

    gdt[index].limit_low   = limit & 0xFFFF;
    gdt[index].granularity = ((limit >> 16) & 0x0F) | (flags & 0xF0);

    gdt[index].access      = access;
}

void gdt_init(void)
{
    gdt_ptr.limit = sizeof(gdt) - 1;
    gdt_ptr.base  = (unsigned int)&gdt;

    gdt_set_entry(0, 0, 0, 0, 0);               // null descriptor
    gdt_set_entry(1, 0, 0xFFFFF, 0x9A, 0xCF);   // code: 0x08
    gdt_set_entry(2, 0, 0xFFFFF, 0x92, 0xCF);   // data: 0x10

    gdt_flush((unsigned int)&gdt_ptr);
}

