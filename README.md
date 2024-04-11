CRYPTHANTUS OS
---

x86 operating system I am making just for fun :>

This OS is tested and run on the QEMU emulator. Currently, its features are very, very limited. However, that will change as the OS develops further.

A screenshot of the OS on the QEMU emulator.

<img src='https://github.com/Felix1G/CrypthantusOS/assets/87594136/734ccbfb-6df5-4762-b196-7ae02b12308e' width='500' alt='Screenshot of the OS on the emulator.'>

---
**MEMORY**

The Buddy Allocator is used with paging. All programs use a shared heap.

---

**SYSCALL TABLE**
```nasm
; Example of calling a syscall in Cryptantus OS
mov  eax, 2
mov  ebx, hello_world
mov  ecx, esp
int  88h                 ; call printf

; ...

hello_world: db "Hello, World!", 0xA, 0x0
```
| Syscall Number | Registers | Description |
| :-: | :- | :- |
| 0x00 | ebx = exit code | End the program. |
| 0x01 | ebx = format string<br/>ecx = stack address of variables | Read from the console. (scanf) |
| 0x02 | ebx = format string<br/>ecx = stack address of variables | Print to the console. (printf) |
| 0x03 | ebx = time in ms | Sleep in ms. |
| 0x04 | ebx = size in bytes | Heap allocation: malloc(ebx). |
| 0x05 | ebx = size in bytes | Similar to syscall 0x04, but values are set to zero. |
| 0x06 | ebx = size in bytes<br/>ecx = address of memory to realloc | Heap reallocation: realloc(ecx, ebx). |
| 0x07 | ebx = address of memory to free | Heap free: free(ebx). |

Note: currently, only syscall 0x00 and 0x02 is available. Others are under construction.

Current plans:
- Support for the hard disk, sounds, and graphics
- A programming language specially for it (?)
- A game.
