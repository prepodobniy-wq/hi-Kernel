// src/timer.c
// PIT timer: ticks counter

#include "ports.h"
#include "isr.h"
#include "irq.h"
#include "timer.h"

#define PIT_COMMAND   0x43
#define PIT_CHANNEL0  0x40
#define PIT_BASE_FREQ 1193182

static volatile unsigned int timer_ticks = 0;

static void timer_handler(struct registers *regs)
{
    (void)regs;
    timer_ticks++;
}

void timer_init(unsigned int frequency)
{
    unsigned int divisor;

    if (frequency == 0) {
        return;
    }

    irq_register(0, timer_handler);

    divisor = PIT_BASE_FREQ / frequency;

    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
}

unsigned int timer_get_ticks(void)
{
    return timer_ticks;
}
