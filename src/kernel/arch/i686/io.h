#pragma once

#include <stdint.h>

#define ASMCALL __attribute__((cdecl))

void ASMCALL i686_outb(uint16_t port, uint8_t value);
uint8_t ASMCALL i686_inb(uint16_t port);
void ASMCALL i686_outw(uint16_t port, uint16_t value);
void ASMCALL i686_enable_interrupts(void);
void ASMCALL i686_disable_interrupts(void);
void ASMCALL i686_switch_ring_mode(uint8_t ring);

uint32_t ASMCALL i686_kernel_stack_segment(void);
uint32_t ASMCALL i686_kernel_stack_pointer(void);

//returns the character read after waiting
int ASMCALL i686_io_wait_keyboard(void);

void i686_io_wait();
int i686_io_keyboard_read();