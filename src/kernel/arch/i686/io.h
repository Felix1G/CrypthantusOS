#pragma once

#include <stdint.h>

#define ASMCALL __attribute__((cdecl))

void ASMCALL i686_outb(uint16_t port, uint8_t value);
uint8_t ASMCALL i686_inb(uint16_t port);
uint32_t ASMCALL i686_inl(uint16_t port);
void i686_insl(uint16_t port, uint32_t* buffer, int quads); //convert to quads later
void ASMCALL i686_outw(uint16_t port, uint16_t value);
void ASMCALL i686_enable_interrupt(void);
void ASMCALL i686_disable_interrupt(void);
void ASMCALL i686_switch_ring_mode(uint8_t ring);

void ASMCALL i686_halt();

uint32_t ASMCALL i686_kernel_stack_segment(void);
uint32_t ASMCALL i686_kernel_stack_pointer(void);

void ASMCALL i686_wait_interrupt(void);
int ASMCALL i686_wait_keyboard_interrupt(void);
void i686_io_wait();
int i686_io_keyboard_read();

int ASMCALL i686_switch_to_ring3(uint32_t user_entry, uint32_t user_func, uint32_t user_stack, uint32_t process_num);
void ASMCALL i686_user_entry();