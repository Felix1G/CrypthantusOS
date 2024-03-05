#pragma once

#define i686_GDT_CODE_SEG 0x08
#define i686_GDT_DATA_SEG 0x10

void i686_gdt_init();
