#include "irq.h"
#include "pic.h"
#include "io.h"
#include <stddef.h>
#include <stdio.h>

#define PIC_REMAP_OFFSET 0x20

IRQ_HANDLER irq_handlers[16];

void i686_irq_handler(REGISTERS* regs)
{
    int irq = regs->intr - PIC_REMAP_OFFSET;
    
    uint8_t pic_isr = i686_pic_read_isr();
    uint8_t pic_irr = i686_pic_read_irr();

    if (irq_handlers[irq] != NULL)
    {
        irq_handlers[irq](regs);
    }
    else
    {
        printf("[ERROR]Unhandled Exception %u\nISR=0x%X   IRR=0x%X   ERR=0x%8X\n", irq, pic_isr, pic_irr, regs->err);
    }

    i686_pic_send_eoi(irq);
}

void i686_reg_pic_send_eoi(REGISTERS* regs)
{
    int irq = regs->intr - PIC_REMAP_OFFSET;
    i686_pic_send_eoi(irq);
}

#define MS_PER_IDLE_TICK (50)

PREEMPT_HANDLER preempt_handler = NULL;
volatile int idle_ticks = 0;
volatile int ticks = 0;

void irq_idle(REGISTERS* regs) { //called once every 50ms, 20tps
    idle_ticks--;
    ticks++;
    
    if (preempt_handler != NULL)
    {
        i686_disable_interrupt();
        if (preempt_handler(regs, ticks))
            ticks = 0;
        i686_enable_interrupt();
    }
}

void i686_sleep(int ms)
{
    idle_ticks = ms / MS_PER_IDLE_TICK; //ticks
    while (idle_ticks > 0) {
        i686_halt();
    }
}

volatile int interrupt_called = 0;
volatile int interrupt_called_ready = 0;
IRQ_HANDLER old_handler;
void sleep_interrupt_irq(REGISTERS* regs)
{
    interrupt_called = 1;
}

void i686_wait_ready(int irq)
{
    if (interrupt_called_ready)
        return;
        
    interrupt_called = 0;
    interrupt_called_ready = 1;
    old_handler = irq_handlers[irq];
    i686_irq_reg_handler(irq, sleep_interrupt_irq);
}

void i686_wait(int irq)
{
    i686_wait_max_time(irq, -1);
}

void i686_wait_max_time(int irq, int ms)
{
    if (!interrupt_called_ready)
        i686_wait_ready(irq);
    
    idle_ticks = ms / MS_PER_IDLE_TICK;
    while (!interrupt_called && (ms < 0 || idle_ticks != 0))
        i686_halt();

    interrupt_called = 0;
    interrupt_called_ready = 0;

    i686_irq_reg_handler(irq, old_handler);
}

void i686_irq_init()
{
    i686_pic_config(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8);

    //register handlers for each irq line
    for (int i = 0; i < 16; i++)
        i686_isr_reg_handler(PIC_REMAP_OFFSET + i, i686_irq_handler);
    i686_irq_reg_handler(0, irq_idle);

    i686_enable_interrupt();
}

void i686_irq_reg_handler(int irq, IRQ_HANDLER handler)
{
    irq_handlers[irq] = handler;
}

void i686_preempt_handler(PREEMPT_HANDLER handler)
{
    preempt_handler = handler;
}