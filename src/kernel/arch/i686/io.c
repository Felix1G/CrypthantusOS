#include "io.h"
#include <stdio.h>

#define UNUSED_PORT 0x80
#define KEYBOARD_DAT_PORT 0x60

void i686_insl(uint16_t port, uint32_t* buffer, int quads)
{
    for (int i = 0;i < quads;i++) {
        buffer[i] = i686_inl(port);
    }
}

void i686_io_wait()
{
    i686_outb(UNUSED_PORT, 0);
}

int i686_io_keyboard_read()
{
    return i686_inb(KEYBOARD_DAT_PORT);
}