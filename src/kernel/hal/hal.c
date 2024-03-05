#include "hal.h"

void hal_init()
{
    printf("[LOG]Initialising Global Descriptor Table.");
    i686_gdt_init();
    
    printf("[LOG]Initialising Instruction Descriptor Table.");
    i686_idt_init();
    
    printf("[LOG]Initialising Interrupt Service Routine.");
    i686_isr_init();
    i686_isr_reg_handler(13, _hal_isr_gpf);

    printf("[LOG]Initialising Interrupt Request.");
    i686_irq_reg_handler(0, _hal_irq_idle);
    i686_irq_reg_handler(1, _hal_keyboard_irq_handler);
    i686_irq_init();
}

int hal_inb(int port)
{
    return (int)i686_inb((uint16_t)port);
}

void hal_outb(int port, int value)
{
    i686_outb((uint16_t)port, (uint8_t)value);
}

int hal_io_wait_keyboard()
{
    return i686_io_wait_keyboard();
}

void hal_run_user(void (*user_func)())
{
    //TODO
}

void hal_switch_kernel()
{
    //TODO
}

void hal_shutdown()
{
    i686_outw((uint16_t)0x604, (uint16_t)0x2000);
}