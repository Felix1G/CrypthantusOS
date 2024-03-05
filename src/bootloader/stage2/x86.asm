%macro x86_enter_real_mode 0
    [bits 32]
    jmp     word 18h:.pmode16

.pmode16:
    [bits 16]
    mov     eax, cr0            ; disable protected mode
    and     al, 0
    mov     cr0, eax

    jmp     word 00h:.rmode

.rmode:
    mov     ax, 0               ; setup segments
    mov     ds, ax
    mov     ss, ax

    sti                         ; enable interrupts
%endmacro

%macro x86_enter_protected_mode 0
    cli

    mov     eax, cr0
    or      al, 1
    mov     cr0, eax

    jmp     dword 08h:.pmode

.pmode:
    [bits 32]

    ; setup segments
    mov     ax, 0x10
    mov     ds, ax
    mov     ss, ax
%endmacro

; Convert linear address to segment:offset address format
; Arguments:
;   1 - linear address
;   2 - target segment (out)
;   3 - target 32-bit register to be used
;   4 - target lower 16-bit half of %3
%macro linear_to_segment_offset 4
    mov     %3, %1
    shr     %3, 4
    mov     %2, %4
    mov     %3, %1
    and     %3, 0xf
%endmacro

global x86_outb
x86_outb:
    [bits 32]
    mov     dx, [esp + 4]
    mov     al, [esp + 8]
    out     dx, al
    ret

global x86_inb
x86_inb:
    [bits 32]
    mov     dx, [esp + 4]
    xor     eax, eax
    in      al, dx
    ret

global x86_disk_reset
x86_disk_reset:
    [bits 32]
    push    ebp
    mov     ebp, esp

    x86_enter_real_mode
    [bits 16]

    mov     ah, 0
    mov     dl, [bp + 8]        ; dl - drive
    stc
    int     13h

    ; return value
    mov     eax, 1
    sbb     eax, 0              ; 1 on success, 0 on fail

    push    eax
    x86_enter_protected_mode
    [bits 32]
    pop     eax

    mov     esp, ebp
    pop     ebp
    ret

global x86_disk_read
x86_disk_read:
    [bits 32]
    push    ebp
    mov     ebp, esp

    x86_enter_real_mode
    [bits 16]

    push    ebx
    push    es

    mov     dl, [bp + 8]        ; dl - drive
    mov     ch, [bp + 12]        ; ch - cylinder (lower 8 bits)
    mov     cl, [bp + 13]        ; cl - cylinder to bit 6-7
    shl     cl, 6
    mov     al, [bp + 16]        ; cl - sectors to bits 0-5
    and     al, 3Fh             ; remove bit 7-8
    or      cl, al

    mov     dh, [bp + 20]       ; dh - head
    
    mov     al, [bp + 24]       ; al - count
    
    linear_to_segment_offset [bp + 28], es, ebx, bx

    ; interrupt
    mov     ah, 02h
    stc
    int     13h

    ; return value
    mov     eax, 1
    sbb     eax, 0               ; 1 on success, 0 on fail

    pop     es
    pop     ebx
    
    push    eax
    x86_enter_protected_mode
    [bits 32]
    pop     eax

    mov     esp, ebp
    pop     ebp
    ret

global x86_disk_drive_params
x86_disk_drive_params:
    [bits 32]

    push    ebp
    mov     ebp, esp

    x86_enter_real_mode
    [bits 16]

    push    di                  ; save register values
    push    es
    push    esi
    push    bx

    mov     dl, [bp + 8]        ; dl - drive
    mov     ah, 08h
    mov     di, 0               ; es:di - 0000:0000
    mov     es, di
    stc
    int     13h

    mov     eax, 1
    sbb     eax, 0               ; success = 1, fail = 0

    ; drive type
    linear_to_segment_offset [bp + 12], es, esi, si
    mov     [es:si], bl

    mov     bl, ch              ; cylinders
    mov     bh, cl              ; bit 6-7
    shr     bh, 6
    inc     bx

    linear_to_segment_offset [bp + 16], es, esi, si
    mov     [es:si], bx

    xor     ch, ch              ; sectors - lower 5 bits in cl
    and     cl, 3Fh
    
    linear_to_segment_offset [bp + 20], es, esi, si
    mov     [es:si], cx

    mov     cl, dh              ; heads
    inc     cx
    
    linear_to_segment_offset [bp + 24], es, esi, si
    mov     [es:si], cx

    pop     bx
    pop     esi
    pop     es
    pop     di
    
    push    eax
    x86_enter_protected_mode
    [bits 32]
    pop     eax

    mov     esp, ebp
    pop     ebp
    ret

e820_signature     equ 0x534D4150

global x86_e820_next_block
x86_e820_next_block:
    [bits 32]
    push    ebp
    mov     ebp, esp

    x86_enter_real_mode
    [bits 16]

    ; save registers
    push    ebx
    push    ecx
    push    edx
    push    esi
    push    edi
    push    ds
    push    es

    ; set up parameters
    linear_to_segment_offset [bp + 8], es, edi, di      ; es:di pointer to memory block
    linear_to_segment_offset [bp + 12], ds, esi, si     ; ebx - pointer to continue_id
    mov     ebx, ds:[si]

    mov     eax, 0xE820
    mov     edx, e820_signature
    mov     ecx, 24

    int     0x15

    cmp     eax, e820_signature
    jne     .x86_e820_next_block_error

    mov     eax, ecx            ; return size
    mov     ds:[si], ebx        ; fill continuation param
    jmp     .x86_e820_next_block_end

.x86_e820_next_block_error:
    mov     eax, -1

.x86_e820_next_block_end:
    ; reload registers
    pop     es
    pop     ds
    pop     edi
    pop     esi
    pop     edx
    pop     ecx
    pop     ebx
    
    push    eax
    x86_enter_protected_mode
    [bits 32]
    pop     eax

    mov     esp, ebp
    pop     ebp
    ret