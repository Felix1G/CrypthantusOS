/**
 * Crypthantus OS stdio
 * 
 * Due to the use of stdio as the main io stream for the console,
 * print and scan functions may not align with their original usage.
 * Starting with [LOG], [ERROR], and [DEBUG] for the printf format will activate logging for printf.
 * This will add a log prefix and an EOL at the end of the print.
 * 
 * Firstly, print and scan is limited to only the specifier for simplicity with an exception.
 * printf supports the digit specifier, for example, "%08X" which will be 8 digits of capital hex.
 * I've only put %s, %d, %u in scanf. This is due to the fact that only those are needed for now. 
 * More specifiers will be added in the future.
 * 
 * Secondly, any characters printed with printf cannot be removed (backspace)
 * unless it goes above the screen (scrollback function). This is to prevent
 * the user to backspace anything printed already.
 * 
 * Thirdly, putc is special. It not only puts a character in the console, but
 * also process special 'characters'. These characters, such as up arrow, left arrow,
 * and similar characters will move the cursor. Backspace characters remove the character
 * before the cursor and moves the cursor backwards by 1.
*/
#pragma once

#define NULL (void*)0

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a < b ? b : a)

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

void set_screen_pos(int scr_x, int scr_y);
void clrscr();
void putc(int c);
void puts(const char* str);
int printf(const char* fmt, ...);
int scanf(const char* fmt, ...);

void log(const char* fmt, ...);
void debug(const char* fmt, ...);
void err(const char* fmt, ...);

void io_enable_echo(int en);
int io_echo();