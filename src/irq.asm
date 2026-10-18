; src/irq.asm
; placeholders for IRQ0-IRQ15(vec 32-47)

%macro IRQ 2
global irq%1
irq%1:
    push dword 0       ; false err code
    push dword %2      ; vector number
    jmp irq_common
%endmacro

section .text
extern irq_handler

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47


; general
irq_common:
    pusha
    cld
    push esp
    call irq_handler
    add esp, 4
    popa
    add esp, 8
    iret

; stub table (irq)
section .data
global irq_stub_table
irq_stub_table:
%assign i 0
%rep 16
    dd irq %+ i
%assign i i+1
%endrep

