#include "irq.h"
#include "pic.h"
#include "io.h"
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
        printf("[ERROR] Unhandled Exception %u\nISR=%X\tIRR=%X\n", irq, pic_isr, pic_irr);
    }

    i686_pic_send_eoi(irq);
}

void i686_irq_init()
{
    i686_pic_config(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8);

    //register handlers for each irq line
    for (int i = 0; i < 16; i++)
        i686_isr_reg_handler(PIC_REMAP_OFFSET + i, i686_irq_handler);

    i686_enable_interrupts();
}

void i686_irq_reg_handler(int irq, IRQ_HANDLER handler)
{
    irq_handlers[irq] = handler;
}