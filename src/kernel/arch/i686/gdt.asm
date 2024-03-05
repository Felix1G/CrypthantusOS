bits 32

global i686_gdt_load
i686_gdt_load:
    push    ebp
    mov     ebp, esp

    ; load the gdt
    mov     eax, [ebp + 8]
    lgdt    [eax]

    ; code segment reload
    mov     eax, [ebp + 12]
    push    eax
    push    .code_seg_reload
    retf

.code_seg_reload:
    ; data segment reload
    mov     ax, [ebp + 16]
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    mov     esp, ebp
    pop     ebp,
    ret