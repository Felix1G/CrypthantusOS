bits 16

section .entry

extern __bss_start
extern __end

extern main

global entry
entry:
    cli

    mov     [boot_drive], dl
    
    ; set up the stack
    mov     ax, ds
    mov     ss, ax
    mov     sp, 0xFFF0
    mov     bp, sp

    sti
    cli
    call    enable_A20
    call    load_GDT

    ; enable protected mode flag
    mov     eax, cr0
    or      eax, 1
    mov     cr0, eax

    jmp     dword 08h:pmode

pmode:
    [bits 32]

    ; set up registers
    mov     ax, 0x10
    mov     ds, ax
    mov     ss, ax

    mov     edi, __bss_start
    mov     ecx, __end
    sub     ecx, edi
    mov     al, 0
    cld
    rep     stosb

    xor     edx, edx
    mov     dl, [boot_drive]
    push    edx
    call    main

    cli
    hlt

enable_A20:
    [bits 16]

    call    input_A20_wait
    mov     al, keyboard_controller_disable
    out     keyboard_controller_command_port, al

    call    input_A20_wait
    mov     al, keyboard_controller_read_ctrl_out_port
    out     keyboard_controller_command_port, al

    call    output_A20_wait
    in      al, keyboard_controller_data_port
    push    eax

    call    input_A20_wait
    mov     al, keyboard_controller_write_ctrl_out_port
    out     keyboard_controller_command_port, al

    call    input_A20_wait
    pop     eax
    or      al, 2
    out     keyboard_controller_data_port, al

    call    input_A20_wait
    mov     al, keyboard_controller_enable
    out     keyboard_controller_command_port, al

    call    input_A20_wait
    ret

input_A20_wait:
    [bits 16]
    in      al, keyboard_controller_command_port
    test    al, 2
    jnz     input_A20_wait
    ret

output_A20_wait:
    [bits 16]
    in      al, keyboard_controller_command_port
    test    al, 1
    jz      output_A20_wait
    ret

load_GDT:
    [bits 16]
    cli
    pusha
    lgdt    [GDT_desc]
    sti
    popa
    ret

keyboard_controller_data_port:              equ 0x60
keyboard_controller_command_port:           equ 0x64
keyboard_controller_disable:                equ 0xAD
keyboard_controller_enable:                 equ 0xAE
keyboard_controller_read_ctrl_out_port:     equ 0xD0
keyboard_controller_write_ctrl_out_port:    equ 0xD1

GDT:
    ; NULL
    dq 0

    ; 32b code segment
    dw 0FFFFh               ; limit, full 32b range
    dw 0                    ; base (0-15b)
    db 0                    ; base (16-23b)
    db 10011010b            ; access (present, ring 0, code segment, executable, direction 0, readable)
    db 11001111b            ; granularity (4k pages, 32b pmode) + limit (16-19b)
    db 0

    ; 32b data segment
    dw 0FFFFh               ; limit, full 32b range
    dw 0                    ; base (0-15b)
    db 0                    ; base (16-23b)
    db 10010010b            ; access (present, ring 0, data segment, executable, direction 0, readable)
    db 11001111b            ; granularity (4k pages, 32b pmode) + limit (16-19b)
    db 0

    ; 16b code segment
    dw 0FFFFh               ; limit, full 32b range
    dw 0                    ; base (0-15b)
    db 0                    ; base (16-23b)
    db 10011010b            ; access (present, ring 0, code segment, executable, direction 0, readable)
    db 00001111b            ; granularity (1b pages, 16b pmode) + limit (16-19b)
    db 0

    ; 16b data segment
    dw 0FFFFh               ; limit, full 32b range
    dw 0                    ; base (0-15b)
    db 0                    ; base (16-23b)
    db 10010010b            ; access (present, ring 0, data segment, executable, direction 0, readable)
    db 00001111b            ; granularity (1b pages, 16b pmode) + limit (16-19b)
    db 0

GDT_desc:
    dw GDT_desc - GDT       ; size of GDT
    dd GDT                  ; address of GDT

boot_drive: db 0