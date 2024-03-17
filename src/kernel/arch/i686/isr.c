#include "isr.h"
#include "idt.h"
#include <stddef.h>
#include <stdio.h>

ISR_HANDLER isr_handlers[256];

const char* isr_exception_msg[] = {
    "division by zero",
    "debug exception",
    "non-maskable interrupt",
    "breakpoint",
    "number overflow",
    "out of bounds",
    "invalid opcode",
    "unavailable device",
    "double fault",
    "coprocessor segment overrun",
    "invalid TSS",
    "segment not present",
    "stack-segment fault",
    "general protection fault",
    "page fault",
    "", //reserved
    "x87 floating-point exception",
    "alignment check",
    "machine check",
    "SIMD floating-point exception",
    "virtualisation exception",
    "control protection exception",
    "",
    "",
    "",
    "",
    "",
    "",
    "hypervisor injection exception",
    "VMM communication exception",
    "security exception"
};

void i686_panic();

void i686_isr_init_gates();

void i686_isr_init()
{
    i686_isr_init_gates();

    for (int i = 0;i < 256;i++)
        i686_idt_set_present_flag(i);

    i686_idt_unset_present_flag(0x80);
}

void __attribute__((cdecl)) i686_isr_handler(REGISTERS* regs)
{
    if (isr_handlers[regs->intr] != NULL)
        isr_handlers[regs->intr](regs);
    else if (regs->intr >= 32)
        printf("[ERROR] Unhandled Interrupt %u.\n", regs->intr);
    else
    {
        printf("[ERROR] Unhandled Exception %u: %s.\n\n\
        interrupt code - %u\n\
        error code     - %u\n\n\
        Register Values:\n\
        eax=%u  ebx=%u  ecx=%u\n\
        edx=%u  esi=%u  edi=%u\n\
        esp=%u  ebp=%u  eip=%u  eflags=%u\n\
        cs=%u  ds=%u  ss=%u\n", 
        regs->intr, isr_exception_msg[regs->intr], regs->intr, regs->err, 
        regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi,
        regs->esp, regs->ebp, regs->eip, regs->eflags,
        regs->cs, regs->ds, regs->ss);

        i686_panic();
    }
}

void i686_isr_reg_handler(int intr, ISR_HANDLER handler)
{
    isr_handlers[intr] = handler;
    i686_idt_set_present_flag(intr);
}