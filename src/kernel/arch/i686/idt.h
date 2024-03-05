#pragma once
#include <stdint.h>

typedef enum
{
    IDT_FLAG_GATE_TASK      = 0x5,
    IDT_FLAG_GATE_16B_INT   = 0x6,
    IDT_FLAG_GATE_16B_TRAP  = 0x7,
    IDT_FLAG_GATE_32B_INT   = 0xE,
    IDT_FLAG_GATE_32B_TRAP  = 0xF,

    IDT_FLAG_RING0          = (0x0 << 5),
    IDT_FLAG_RING1          = (0x1 << 5),
    IDT_FLAG_RING2          = (0x2 << 5),
    IDT_FLAG_RING3          = (0x3 << 5),

    IDT_FLAG_PRESENT        = 0x80
} IDT_FLAGS;

void i686_idt_init();
void i686_idt_set_present_flag(int intr);
void i686_idt_unset_present_flag(int intr);
void i686_idt_descriptor(int intr, void* base, uint16_t seg_desc, uint8_t flags);
