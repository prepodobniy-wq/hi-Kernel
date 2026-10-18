// src/ports.h
// access to I/O ports

#ifndef PORTS_H
#define PORTS_H

static inline void outb(unsigned short port, unsigned char value)
{
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline unsigned char inb(unsigned short port)
{
    unsigned char result;

    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

#endif
