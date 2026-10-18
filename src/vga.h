// src/vga.h
// functions for displaying text on the screen

#ifndef VGA_H
#define VGA_H

void vga_clear(void);
void vga_put_char(char c);
void vga_print(const char *text);

#endif
