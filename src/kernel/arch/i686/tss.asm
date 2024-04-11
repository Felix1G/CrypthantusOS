bits 32

global flush_tss
flush_tss:
    mov     ax, 0x28    ; fifth 8-byte selector, ring 0
    ltr     ax
    ret