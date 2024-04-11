global i686_outb
i686_outb:
    [bits 32]
    mov     dx, [esp + 4]
    mov     al, [esp + 8]
    out     dx, al
    ret

global i686_inb
i686_inb:
    [bits 32]
    mov     dx, [esp + 4]
    xor     eax, eax
    in      al, dx
    ret

global i686_inl
i686_inl:
    [bits 32]
    mov     dx, [esp + 4]
    xor     eax, eax
    in      eax, dx
    ret

global i686_outw
i686_outw:
    [bits 32]
    mov     dx, [esp + 4]
    mov     ax, [esp + 8]
    out     dx, ax
    ret

global i686_halt
i686_halt:
    sti
    hlt

global i686_enable_interrupt
i686_enable_interrupt:
    sti
    ret

global i686_disable_interrupt
i686_disable_interrupt:
    cli
    ret

global i686_wait_interrupt
i686_wait_interrupt:
    sti
    hlt
    ret

global i686_wait_keyboard_interrupt
i686_wait_keyboard_interrupt:
    push eax
    mov al, 0xFC    ; mask except keyboard and idle (for preemption)
    out 0x21, al

    sti
    hlt

    mov al, 00
    out 0x21, al
    pop eax

    ret

global i686_kernel_stack_segment
i686_kernel_stack_segment:
    mov     eax, ss
    ret

global i686_kernel_stack_pointer
i686_kernel_stack_pointer:
    mov     eax, esp
    ret

global i686_user_entry
i686_user_entry:
    call    eax
    mov     ebx, eax            ; set exit code
    mov     eax, 0x00           ; end program call
    int     88h

.loop_wait:                     ; wait until this process is pre-empted so that
    jmp     .loop_wait          ; it will never run again

    ret                         ; should not happen