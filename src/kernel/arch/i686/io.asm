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

global i686_outw
i686_outw:
    [bits 32]
    mov     dx, [esp + 4]
    mov     ax, [esp + 8]
    out     dx, ax
    ret

global i686_enable_interrupts
i686_enable_interrupts:
    sti
    ret

global i686_disable_interrupts
i686_disable_interrupts:
    cli
    ret

global i686_switch_ring_mode
i686_switch_ring_mode:
    ; TODO
    ret

extern i686_io_keyboard_read
global i686_io_wait_keyboard
i686_io_wait_keyboard:
    push    eax                 ; save eax

    mov     al, 0xFD            ; mask all interrupts except IRQ 1
    out     21h, al

    sti
    hlt

    mov     al, 0h              ; reenable all
    out     21h, al

    pop     eax                 ; read saved eax

    call    i686_io_keyboard_read
    ret

global i686_kernel_stack_segment
i686_kernel_stack_segment:
    mov     eax, ss
    ret

global i686_kernel_stack_pointer
i686_kernel_stack_pointer:
    mov     eax, esp
    ret