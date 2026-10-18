// src/keyboard.c
// keyboard driver

#include "ports.h"
#include "isr.h"
#include "irq.h"
#include "keyboard.h"

#define KEYBOARD_DATA_PORT 0x60
#define SCANCODE_COUNT     58
#define BUFFER_SIZE        64

// scan-code (set 1) -> symbol
// 0 means "no symbol"
static const char scancode_table[SCANCODE_COUNT] = {
    0,   0,   '1', '2', '3', '4', '5', '6',   // 0x00-0x07
    '7', '8', '9', '0', '-', '=', 0,   0,     // 0x08-0x0F
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',   // 0x10-0x17
    'o', 'p', '[', ']', '\n', 0, 'a', 's',    // 0x18-0x1F
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',   // 0x20-0x27
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',   // 0x28-0x2F
    'b', 'n', 'm', ',', '.', '/', 0,   '*',   // 0x30-0x37
    0,   ' '
};

static char buffer[BUFFER_SIZE];
static volatile unsigned int head = 0;
static volatile unsigned int tail = 0;

static void keyboard_handler(struct registers *regs)
{
    unsigned char scancode;
    char c;
    unsigned int next;

    (void)regs;

    scancode = inb(KEYBOARD_DATA_PORT);

    if (scancode & 0x80) {
        return;
    }

    if (scancode >= SCANCODE_COUNT) {
        return;
    }

    c = scancode_table[scancode];
    if (c == 0) {
        return;
    }

    next = (head + 1) % BUFFER_SIZE;
    if (next != tail) {
        buffer[head] = c;
        head = next;
    }
}

void keyboard_init(void)
{
    irq_register(1, keyboard_handler);
}

char keyboard_get_char(void)
{
    char c;

    if (head == tail) {
        return 0;
    }

    c = buffer[tail];
    tail = (tail + 1) % BUFFER_SIZE;
    return c;
}
