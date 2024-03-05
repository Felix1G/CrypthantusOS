#include "io.h"

#define UNUSED_PORT 0x80
#define KEYBOARD_DAT_PORT 0x60

void i686_io_wait()
{
    i686_outb(UNUSED_PORT, 0);
}

int i686_io_keyboard_read()
{
    return i686_inb(KEYBOARD_DAT_PORT);
}