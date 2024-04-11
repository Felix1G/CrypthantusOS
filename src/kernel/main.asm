global user_func
user_func:
    mov eax, 123
    int 88h
    mov eax, 564
    int 88h
.a:
    mov eax, 564
    int 88h
    cmp ecx, 12030
    jne .a
    mov eax, 564
    int 88h
    ret
    
global user_func2
user_func2:
    mov eax, 2

    mov ebx, hello_world_str

    push dword hello_world
    mov edx, 123
    push edx
    mov ecx, esp

    int 88h

    pop eax
    pop eax

    mov eax, 0
    ret

hello_world: db "Hello, World!", 0xA, 0x0
hello_world_str: db "Hello, World! Your Value: %i '%s'", 0xA, 0x0