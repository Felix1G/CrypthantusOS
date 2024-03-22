#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stack.h>
#include "hal/hal.h"
#include "util/cmd.h"
#include "util/buddy_alloc.h"
#include <boot/bootlib.h>

extern uint32_t __bss_start;
extern uint32_t __kernel_end;

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
    printf("No more doing weird stuff.");
}

typedef struct 
{
    int a, b, c, d, e;
} TEST_STRUCT;

TEST_STRUCT* init_test_struct(int rand)
{
    int block_size;
    TEST_STRUCT* test = buddy_alloc(10 * sizeof(TEST_STRUCT), &block_size);
    for (int i = 0;i < 10;i++)
    {
        test[i].a = (2 * rand) << i;
        test[i].b = (4 * rand) << i;
        test[i].c = (5 * rand) << i;
        test[i].d = (7 * rand) << i;
        test[i].e = (9 * rand) << i;
    }
    return test;
}

void __attribute__((section(".entry"))) main(uint32_t scr_x, uint32_t scr_y, BOOT_DATA* boot_data)
{
    memset(&__bss_start, 0, (&__kernel_end) - (&__bss_start));
    set_screen_pos(scr_x, scr_y);

    printf("[LOG]Initialising HAL.");
    hal_init();
    hal_keyboard_onpress(keyboard_listener);
    printf("[LOG]HAL initialised.");

    printf("[LOG]Initialising Buddy Allocator.");
    int buddy_mem;
    if (buddy_mem = buddy_init(boot_data))
    {
        printf("[LOG]Buddy Allocator initialised. %iKB memory allocated.",
            buddy_mem >> 10);
    }
    else
    {
        printf("[ERROR]Buddy Allocator initialisation failed. Kernel ends here.");
        goto end;
    }

    printf("[LOG]Initialising Heap.");
    int heap_mem;
    if (heap_mem = heap_init())
    {
        printf("[LOG]Heap initialised. %iB allocated for the block addresses.", heap_mem);
    }
    else
    {
        printf("[ERROR]Heap initialisation failed. Kernel ends here.");
        goto end;
    }

    /*printf("[LOG]Initialising Page Directory.");
    hal_page_init();
    printf("[LOG]Page Directory Initialised.");*/

    printf("[LOG]Initialising Floppy Disk Driver.");
    if (hal_disk_init(&boot_data->fat_data, &boot_data->disk, 0))
    {
        printf("[LOG]Floppy Disk Driver initialisation failed.");
        goto end;
    }
    else
    {
        printf("[LOG]Floppy Disk Driver initialised.");
    }

    int block_size;
    char buffer[1024];
    char command[1024];
    STACK* stack = stack_init(16 * 1024, sizeof(unsigned));
    if (stack == NULL)
    {
        printf("[DEBUG]Stack cannot be initialised.");
        goto end;
    }
    
    printf("[LOG]Commands available.");
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
            printf("memmap  \t\t\tprints the memory mapping.\n");
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
            unsigned* a = (unsigned*)stack_pop(stack);
            if (a) {
                printf("0x%X\n", *a);
                free(*a);
                _heap_debug();
            } else {
                printf("0x%X\n", *a);
                printf("NULL\n");
            }
        }
        else if (!strcmp(command, "try1"))
        {
            int size = 0;
            scanf("%d", &size);
            unsigned* a = (unsigned*)stack_pop(stack);
            if (a) {
                *a = realloc((void*)*a, size);
                stack_push(stack, *a);
                printf("0x%X\n", *a);
                _heap_debug();
            } else {
                printf("NULL\n");
            }
        }
        else if (!strcmp(command, "try2"))
        {
            int size = 0;
            scanf("%d", &size);
            uint8_t* p = (uint8_t*)malloc(size);
            if (p == NULL)
                printf("NULL\n");
            else {
                stack_push(stack, p);
                printf("0x%X\n", (unsigned)p);
                for (int i = 0;i < 1000;i++)
                    p[i * 1024] = 123;
                _heap_debug();
            }
        }
        else if (!strcmp(command, "try3"))
        {
            FAT_FILE* fat_file = fat_open(&boot_data->disk, "dir/inner/test.txt");
            if (fat_file != NULL)
            {
                uint32_t read;
                char* buffer = (char*)malloc(15100);
                while ((read = fat_read(&boot_data->disk, fat_file, 15000, buffer)))
                {
                    buffer[read] = '\0';
                    printf(buffer);
                }
                fat_close(fat_file);
            }
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