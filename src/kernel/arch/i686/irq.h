#pragma once
#include "isr.h"

typedef void (*IRQ_HANDLER)(REGISTERS* regs);
typedef int (*PREEMPT_HANDLER)(REGISTERS* regs, int ticks);

void i686_reg_pic_send_eoi(REGISTERS* regs);
void i686_irq_init();
void i686_irq_reg_handler(int irq, IRQ_HANDLER handler);
void i686_preempt_handler(PREEMPT_HANDLER handler);
void i686_sleep(int ms);
void i686_wait(int irq);
void i686_wait_max_time(int irq, int ms);
void i686_wait_ready(int irq);