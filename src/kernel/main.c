#include <stdint.h>
#include "stdio.h"
#include "string.h"
#include "hal/hal.h"
#include "util/cmd.h"
#include <boot/bootlib.h>

extern uint8_t __bss_start;
extern uint8_t __end;

void keyboard_listener(int ch, int up, const KEY_STATE* state)
{
    switch (ch)
    {
    case KEY_UP:
    case KEY_DOWN:
    case KEY_LEFT:
    case KEY_RIGHT:
        if (!up)
            putc(ch);
        return;
    }

    if (isnotchar(ch))
        return;

    if (!(ch & 0x80) && !up)
        putc(ch);
}

void user_func()
{
    //printf("No more doing weird stuff.");
}

void __attribute__((section(".entry"))) main(uint32_t scr_x, uint32_t scr_y, BOOT_DATA* boot_data)
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));
    set_screen_pos(scr_x, scr_y);

    printf("[LOG]Initialising HAL.");
    hal_init();

    hal_keyboard_onpress(keyboard_listener);

    char buffer[1024];
    char command[1024];
    while (true) {
        *buffer = '\0';
        
        scanf("%s", &buffer);

        //get command
        char* buf = buffer;
        while (*buf)
        {
            if (*buf == ' ')
                break;
            command[buf - buffer] = *buf;
            buf++;
        }
        command[buf - buffer] = '\0';

        //run command
        if (!strcmp(command, "help"))
        {
            printf("[LOG]Commands:");
            printf("echo    \t\t\tprint text to console.\n");
            printf("shutdown\t\t\tshuts down the computer.\n");
        }
        else if (!strcmp(command, "echo"))
        {
            cmd_echo(buffer + 4);
        }
        else if (!strcmp(command, "shutdown"))
        {
            if (cmd_shutdown())
                break;
        }
        else if (!strcmp(command, "try"))
        {
            
        }
        else if (!strcmp(command, "memmap"))
        {
            cmd_memmap(boot_data);
        }
        else
        {
            printf("[LOG]%s: not a command.", buffer);
        }
    }

end:
    for (;;);
}