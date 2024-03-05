#pragma once

#include <stdint.h>

void i686_pic_config(uint8_t pic1_offset, uint8_t pic2_offset);
void i686_pic_disable();
void i686_pic_mask(int irq);
void i686_pic_unmask(int irq);
void i686_pic_send_eoi(int irq);
uint16_t i686_pic_read_irr();
uint16_t i686_pic_read_isr();
