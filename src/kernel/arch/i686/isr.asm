bits 32

extern i686_isr_handler

%macro ISR_NOERR_CODE 1
global i686_isr_%1:
i686_isr_%1:
    push    0
    push    %1
    jmp     isr_com
%endmacro

%macro ISR_ERR_CODE 1
global i686_isr_%1:
i686_isr_%1:
    push    %1
    jmp     isr_com
%endmacro

%include "arch/i686/isr_gate.inc"

isr_com:
    pusha

    xor     eax, eax
    mov     ax, ds
    push    eax
    mov     ax, es
    push    eax
    mov     ax, fs
    push    eax
    mov     ax, gs
    push    eax
    mov     eax, cr0
    push    eax
    mov     eax, cr2
    push    eax
    mov     eax, cr3
    push    eax
    mov     eax, cr4
    push    eax

    mov     ax, 0x10        ; use data segment
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    push    esp
    call    i686_isr_handler
    add     esp, 4

    pop     eax
    mov     cr4, eax
    pop     eax
    mov     cr3, eax
    pop     eax
    mov     cr2, eax
    pop     eax
    mov     cr0, eax
    pop     eax
    mov     gs, eax
    pop     eax
    mov     fs, eax
    pop     eax
    mov     es, eax
    pop     eax
    mov     ds, eax

    popa
    add     esp, 8          ; remove err code and intr num

    iret

global i686_panic
i686_panic:
    cli
    hlt