#include "cmd.h"
#include <stdio.h>
#include <string.h>
#include <hal/hal.h>
#include <boot/bootlib.h>

void cmd_echo(const char* arg)
{
    if (*arg)
    {
        while (*arg)
            if (*(arg++) != ' ')
                break;
        arg--;

        if (!memcmp(arg, "on\0", 3)) {
            io_enable_echo(1);
            printf("[LOG]echo is turned on.");
        } else if (!memcmp(arg, "off\0", 4)) {
            io_enable_echo(0);
            printf("[LOG]echo is turned off.");
        } else
            printf("[LOG]%s", arg);
    }
    else
    {
        if (io_echo())
            printf("[LOG]echo is on.");
        else
            printf("[LOG]echo is off.");
    }
}

int cmd_shutdown()
{
    char buffer[4];

    printf("[LOG]are you sure to shutdown? y/n");

    scanf("%s", &buffer);

    if (memcmp(buffer, "y", 1))
        return 0;

    hal_shutdown();
    return 1;
}

const char* g_memory_block_type_str[4] = {
    "UNUSABLE", "RESERVED", "RECLAIMABLE", "NVS"
};

void cmd_memmap(BOOT_DATA* boot_data)
{
    printf("[LOG]Memory Mapping: ");
    for (int i = 0; i < boot_data->memory.blocks_count; i++)
    {
        register MEMORY_BLOCK block = boot_data->memory.blocks[i];
        printf("%08X-%08X\t%s\n", (int)block.begin, (int)(block.begin + block.length - 1),
            g_memory_block_type_str[block.type - 1]);
    }
}
