#pragma once

#define NULL (void*)0

#define min(a, b) a < b ? a : b
#define max(a, b) a < b ? b : a

//0x00000000-0x000003FF: interrupt vector table
//0x00000400-0x000004FF: BIOS data area
#define MEMORY_MIN 0x00000500
#define MEMORY_MAX 0x00080000
//0x00000500-0x00010500: FAT driver
#define MEMORY_FAT_ADDR ((void*)0x20000)
#define MEMORY_FAT_SIZE 0x00010000
//0x00020000-0x00030000: stage2
#define MEMORY_FREE_BUFFER ((void*)0x30000)
#define MEMORY_BUFFER_SIZE 0x10000
//0x00030000-0x00080000: free memory
//0x00080000-0x0009FFFF: extended BIOS data
//0x000A0000-0x000C7FFF: video
//0x000C8000-0x000FFFFF: BIOS
#define MEMORY_KERNEL_ADDR ((void*)0x100000)

void screen_pos(int *src_x, int *scr_y);
void clrscr();
void putc(char c);
void puts(const char* str);
void printf(const char* fmt, ...);
void print_buffer(const char* msg, const void* buffer, unsigned n);