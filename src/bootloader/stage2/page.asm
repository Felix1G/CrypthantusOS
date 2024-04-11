bits 32

global _load_page_dir
_load_page_dir:
    push    ebp
    mov     ebp, esp

    ; load page directory
    mov     eax, [esp + 8]
    mov     cr3, eax

    mov     esp, ebp
    pop     ebp
    ret

global _enable_paging
_enable_paging:
    ; enable paging
    mov     eax, cr0
    or      eax, 0x80000001
    mov     cr0, eax
    ret
