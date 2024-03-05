#pragma once
#include "isr.h"

typedef void (*IRQ_HANDLER)(REGISTERS* regs);

void i686_irq_init();
void i686_irq_reg_handler(int irq, IRQ_HANDLER handler);