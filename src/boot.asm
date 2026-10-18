; src/boot.asm
; Entry point of hi!Kernel

MULTIBOOT_MAGIC       equ 0x1BADB002
MULTIBOOT_FLAGS       equ 0
MULTIBOOT_CHECKSUM    equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

; header for GRUB
section .multiboot
align 4
    dd MULTIBOOT_MAGIC
    dd MULTIBOOT_FLAGS
    dd MULTIBOOT_CHECKSUM

; memory for stack: 16kb
section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
global _start
extern kernel_main

_start:
    mov esp, stack_top      ; setting up stack
    call kernel_main        ; giving control to C-code

.hang:
    cli                     ; disabling interrupts
    hlt                     ; halt the processor
    jmp .hang               ; if wake up, halt again
