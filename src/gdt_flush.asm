; src/gdt_flush.asm
; loading the GDT into the processor and updating segment registers

section .text
global gdt_flush

gdt_flush:
    mov eax, [esp + 4]     ; argument: address of the gdt_ptr structure
    lgdt [eax]             ; loading GDT

    mov ax, 0x10           ; data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.reload_cs    ; far jump: reloading cs with a code selector

.reload_cs:
    ret
