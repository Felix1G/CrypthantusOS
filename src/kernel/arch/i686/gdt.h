#pragma once

#define i686_GDT_CODE_SEG 0x08
#define i686_GDT_DATA_SEG 0x10

void i686_gdt_init();

int i686_is_ring3_tss_gdt_init();
void i686_ring3_tss_gdt_init(uint32_t stack_segment, uint32_t stack_pointer);