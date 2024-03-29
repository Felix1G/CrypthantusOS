#pragma once

#include "fat.h"
#include <stdbool.h>
#include <stdio.h>
#include <arch/i686/isr.h>
#include <util/buddy_alloc.h>

#define KEY_LSHIFT      0x80
#define KEY_RSHIFT      0x81
#define KEY_CAPS        0x82
#define KEY_CTRL        0x83
#define KEY_ALT         0x84
#define KEY_UP          0x85
#define KEY_DOWN        0x86
#define KEY_LEFT        0x87
#define KEY_RIGHT       0x88
#define KEY_ESC         0x1B
#define KEY_BACKSPACE   0x08
#define KEY_DELETE      0x7F
#define KEY_PAGE_UP     0x8C
#define KEY_PAGE_DOWN   0x8D
#define KEY_F1          0x8E
#define KEY_F2          0x8F
#define KEY_F3          0x90
#define KEY_F4          0x91
#define KEY_F5          0x92
#define KEY_F6          0x93
#define KEY_F7          0x94
#define KEY_F8          0x95
#define KEY_F9          0x96
#define KEY_F10         0x97
#define KEY_F11         0x98
#define KEY_F12         0x99

int isnotchar(int ch);

typedef struct
{
    bool caps_lock;
    bool lshift;
    bool rshift;
    bool ctrl;
    bool alt;
} KEY_STATE;

void _hal_isr_zdiv(REGISTERS* regs); //div zero

void hal_init();
void hal_shutdown();

int hal_inb(int port);
void hal_outb(int port, int value);

void _hal_irq_keyboard(REGISTERS* regs);
int hal_io_wait_keyboard(); //returns the character read after waiting
int hal_keyboard_char();
int hal_keyboard_up();
void hal_keyboard_onpress(void (*listener)(int ch, int up, const KEY_STATE* state));

int hal_disk_init(FAT_DATA* data, DISK* disk, int test_read);

//void hal_page_init();
//void _hal_irq_page_fault(REGISTERS* regs);