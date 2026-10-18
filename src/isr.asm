; src/isr.asm
; placeholder to avoid 0 (division by zero)

%macro ISR_NO_ERROR 1
global isr%1
isr%1:
    push dword 0            ; fake error code
    push dword %1           ; exception number
    jmp isr_common
%endmacro


%macro ISR_WITH_ERROR 1
global isr%1
isr%1:
    push dword %1           ; exception number (err code is in stack already)
    jmp isr_common
%endmacro


section .text
global isr0
extern isr_handler


ISR_NO_ERROR   0
ISR_NO_ERROR   1
ISR_NO_ERROR   2
ISR_NO_ERROR   3
ISR_NO_ERROR   4
ISR_NO_ERROR   5
ISR_NO_ERROR   6
ISR_NO_ERROR   7
ISR_WITH_ERROR 8
ISR_NO_ERROR   9
ISR_WITH_ERROR 10
ISR_WITH_ERROR 11
ISR_WITH_ERROR 12
ISR_WITH_ERROR 13
ISR_WITH_ERROR 14
ISR_NO_ERROR   15
ISR_NO_ERROR   16
ISR_WITH_ERROR 17
ISR_NO_ERROR   18
ISR_NO_ERROR   19
ISR_NO_ERROR   20
ISR_WITH_ERROR 21
ISR_NO_ERROR   22
ISR_NO_ERROR   23
ISR_NO_ERROR   24
ISR_NO_ERROR   25
ISR_NO_ERROR   26
ISR_NO_ERROR   27
ISR_NO_ERROR   28
ISR_NO_ERROR   29
ISR_WITH_ERROR 30
ISR_NO_ERROR   31


; general
isr_common:
    pusha
    cld
    push esp
    call isr_handler
    add esp, 4
    popa
    add esp, 8
    iret

; stub table
section .data
global isr_stub_table
isr_stub_table:
%assign i 0
%rep 32
    dd isr %+ i
%assign i i+1
%endrep

