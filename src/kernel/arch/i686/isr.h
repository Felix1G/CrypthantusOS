#pragma once
#include <stdint.h>

typedef struct
{
    uint32_t cr4, cr3, cr2, cr0;
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, oldesp, ebx, edx, ecx, eax; //pusha
    uint32_t intr, err;
    uint32_t eip, cs, eflags, esp, ss; //auto-pushed by CPU
} __attribute__((packed)) REGISTERS;

typedef void (*ISR_HANDLER)(REGISTERS* regs);

void i686_isr_init();
void i686_isr_reg_handler(int intr, ISR_HANDLER handler);
void i686_isr_reset_keyboard_intr();
int i686_isr_get_keyboard_intr();
int i686_isr_recent_interrupt();

void __attribute__((cdecl)) i686_panic();
