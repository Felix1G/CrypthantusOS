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

    i686_idt_unset_present_flag(0x88);
}

volatile int recent_intr = 0;
volatile int is_keyboard_intr = 0;

void i686_isr_reset_keyboard_intr()
{
    is_keyboard_intr = 0;
}

int i686_isr_get_keyboard_intr()
{
    return is_keyboard_intr;
}

int i686_isr_recent_interrupt()
{
    return recent_intr == 33; //hardcoded value :(
}

void __attribute__((cdecl)) i686_isr_handler(REGISTERS* regs)
{
    recent_intr = regs->intr;
    if (recent_intr == 33)
        is_keyboard_intr = 1;

    if (isr_handlers[regs->intr] != NULL)
        isr_handlers[regs->intr](regs);
    else if (regs->intr >= 32)
        printf("[ERROR]Unhandled Interrupt %u.\n", regs->intr);
    else
    {
        printf("[ERROR]Unhandled Exception %u: %s.\n\n\
        interrupt code - %u\n\
        error code     - %u\n\n\
        Register Values:\n\
        eax=0x%X  ebx=0x%X  ecx=0x%X\n\
        edx=0x%X  esi=0x%X  edi=0x%X\n\
        esp=0x%X  ebp=0x%X  eip=0x%X  eflags=0x%X\n\
        cr0=0x%X  cr2=0x%X  cr3=0x%X  cr4=0x%X\n\
        cs=0x%X  ds=0x%X  es=0x%X\n\
        fs=0x%X  gs=0x%X  ss=0x%X\n", 
        regs->intr, isr_exception_msg[regs->intr], regs->intr, regs->err, 
        regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi,
        regs->esp, regs->ebp, regs->eip, regs->eflags,
        regs->cr0, regs->cr2, regs->cr3, regs->cr4,
        regs->cs, regs->ds, regs->es,
        regs->fs, regs->gs, regs->ss);

        i686_panic();
    }
}

void i686_isr_reg_handler(int intr, ISR_HANDLER handler)
{
    isr_handlers[intr] = handler;
    i686_idt_set_present_flag(intr);
}