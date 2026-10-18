# Makefile
# Building and launching hi!Kernel

CC = gcc
AS = nasm
LD = ld

CFLAGS  = -m32 -ffreestanding -fno-stack-protector -fno-pic -Wall -Wextra
LDFLAGS = -m elf_i386 -T linker.ld

all: build/hikernel.iso

build/boot.o: src/boot.asm
	mkdir -p build
	$(AS) -f elf32 src/boot.asm -o build/boot.o

build/kernel.o: src/kernel.c
	mkdir -p build
	$(CC) $(CFLAGS) -c src/kernel.c -o build/kernel.o

build/vga.o: src/vga.c src/vga.h
	mkdir -p build
	$(CC) $(CFLAGS) -c src/vga.c -o build/vga.o

build/string.o: src/string.c src/string.h
	mkdir -p build
	$(CC) $(CFLAGS) -c src/string.c -o build/string.o

build/gdt.o: src/gdt.c src/gdt.h
	mkdir -p build
	$(CC) $(CFLAGS) -c src/gdt.c -o build/gdt.o

build/gdt_flush.o: src/gdt_flush.asm
	mkdir -p build
	$(AS) -f elf32 src/gdt_flush.asm -o build/gdt_flush.o

build/idt.o: src/idt.c src/idt.h
	mkdir -p build
	$(CC) $(CFLAGS) -c src/idt.c -o build/idt.o

build/idt_load.o: src/idt_load.asm
	mkdir -p build
	$(AC) -f elf32 src/idt_load.asm -o build/idt_load.o

build/isr_asm.o: src/isr.asm
	mkdir -p build
	$(AS) -f elf32 src/isr.asm -o build/isr_asm.o

build/isr.o: src/isr.c src/isr.h src/vga.h src/string.h
	mkdir -p build
	$(CC) $(CFLAGS) -c src/isr.c -o build/isr.o

build/pic.o: src/pic.c src/pic.h src/ports.h
	mkdir -p build
	$(CC) $(CFLAGS) -c src/pic.c -o build/pic.o

build/irq_asm.o: src/irq.asm
	mkdir -p build
	$(AS) -f elf32 src/irq.asm -o build/irq_asm.o

build/irq.o: src/irq.c src/irq.h src/isr.h src/ports.h
	mkdir -p build
	$(CC) $(CFLAGS) -c src/irq.c -o build/irq.o

build/timer.o: src/timer.c src/timer.h src/ports.h src/isr.h src/irq.h
	mkdir -p build
	$(CC) $(CFLAGS) -c src/timer.c -o build/timer.o

build/shell.o: src/shell.c src/shell.h src/vga.h src/string.h src/timer.h
	mkdir -p build
	$(CC) $(CFLAGS) -c src/shell.c -o build/shell.o

build/keyboard.o: src/keyboard.c src/keyboard.h src/ports.h src/isr.h src/irq.h
	mkdir -p build
	$(CC) $(CFLAGS) -c src/keyboard.c -o build/keyboard.o

build/kernel.elf: build/boot.o build/kernel.o build/vga.o build/string.o build/gdt.o build/gdt_flush.o build/idt.o build/idt_load.o build/isr_asm.o build/isr.o build/pic.o build/irq_asm.o build/irq.o build/timer.o build/keyboard.o build/shell.o linker.ld
	$(LD) $(LDFLAGS) -o build/kernel.elf build/boot.o build/kernel.o build/vga.o build/string.o build/gdt.o build/gdt_flush.o build/idt.o build/idt_load.o build/isr_asm.o build/isr.o build/pic.o build/irq_asm.o build/irq.o build/timer.o build/keyboard.o build/shell.o

build/hikernel.iso: build/kernel.elf iso/boot/grub/grub.cfg
	cp build/kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o build/hikernel.iso iso

run: build/hikernel.iso
	qemu-system-i386 -cdrom build/hikernel.iso

clean:
	rm -f build/boot.o build/kernel.o build/vga.o build/string.o build/gdt.o build/gdt_flush.o build/idt.o build/idt_load.o build/isr_asm.o build/isr.o build/pic.o build/irq_asm.o build/irq.o build/timer.o build/keyboard.o build/shell.o build/kernel.elf build/hikernel.iso iso/boot/kernel.elf

.PHONY: all run clean
