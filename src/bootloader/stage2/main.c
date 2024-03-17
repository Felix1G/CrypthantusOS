#include <stdint.h>
#include "fat.h"
#include "disk.h"
#include "stdio.h"
#include "string.h"
#include "x86.h"
#include <boot/bootlib.h>

BOOT_DATA g_boot_data;

typedef void __attribute__((cdecl)) (*KERNEL)(int, int, BOOT_DATA*);
uint8_t* kernel_program = (uint8_t*) MEMORY_KERNEL_ADDR;
uint8_t* kernel_buffer = (uint8_t*) MEMORY_FREE_BUFFER;

void detect_memory(MEMORY_INFO* info);

void __attribute__((cdecl)) main(uint16_t boot_drive)
{
    clrscr();
    
    printf("Initialising the disk.\n");

    DISK disk;
    if (!disk_init(&disk, boot_drive))
    {
        printf("An error occurred while initialising the disk. (FATAL)\n");
        return;
    }
    g_boot_data.disk = disk;
    
    printf("Initialising the FAT driver.\n");

    if (!fat_init(&disk))
    {
        printf("An error occurred while initialising the FAT driver. (FATAL)\n");
        return;
    }

    printf("Finding the memory map.\n");
    g_boot_data.boot_device = boot_drive;
    detect_memory(&g_boot_data.memory);

    printf("Searching for kernel.\n");

    FAT_FILE* fat_file = fat_open(&disk, "/kernel.bin");
    if (fat_file == NULL)
    {
        printf("Kernel not found. (FATAL)\n");
        return;
    }

    uint32_t read;
    uint8_t* kernel_prog_pos = kernel_program;
    while ((read = fat_read(&disk, fat_file, MEMORY_BUFFER_SIZE, kernel_buffer)))
    {
        memcpy(kernel_prog_pos, kernel_buffer, read);
        kernel_prog_pos += read;
    }
    fat_close(fat_file);

    printf("Starting kernel.\n");

    int scr_x, scr_y;
    screen_pos(&scr_x, &scr_y);

    KERNEL kernel = (KERNEL)kernel_program;
    kernel(scr_x, scr_y, &g_boot_data);
}

MEMORY_BLOCK g_blocks[256]; //max blocks = 256
int g_blocks_count;

void detect_memory(MEMORY_INFO* info)
{
    MEMORY_BLOCK block;
    uint32_t cont = 0;
    int ret;

    g_blocks_count = 0;
    ret = x86_e820_next_block(&block, &cont);

    while (ret > 0 && cont)
    {
        register MEMORY_BLOCK* mem_block = g_blocks + g_blocks_count;

        mem_block->begin = block.begin;
        mem_block->length = block.length;
        mem_block->type = block.type;
        mem_block->acpi = block.acpi;

        g_blocks_count++;

        ret = x86_e820_next_block(&block, &cont);
    }

    info->blocks = g_blocks;
    info->blocks_count = g_blocks_count;
}
