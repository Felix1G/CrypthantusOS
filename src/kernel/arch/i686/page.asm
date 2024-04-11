bits 32

global _get_page_dir
_get_page_dir:
    ; get page directory
    mov     eax, cr3
    ret