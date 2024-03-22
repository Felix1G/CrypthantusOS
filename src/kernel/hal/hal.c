#include "hal.h"
#include "fat.h"
#include <stdlib.h>
#include <arch/i686/io.h>
#include <arch/i686/gdt.h>
#include <arch/i686/idt.h>
#include <arch/i686/irq.h>
//#include <arch/i686/page.h>
#include <arch/i686/floppy.h>

void hal_init()
{
    printf("[LOG]Initialising Global Descriptor Table.");
    i686_gdt_init();
    
    printf("[LOG]Initialising Instruction Descriptor Table.");
    i686_idt_init();
    
    printf("[LOG]Initialising Interrupt Service Routine.");
    i686_isr_init();
    i686_isr_reg_handler(0, _hal_isr_zdiv);

    printf("[LOG]Initialising Interrupt Request.");
    i686_irq_init();
    i686_irq_reg_handler(1, _hal_irq_keyboard);
    //i686_irq_reg_handler(14, _hal_irq_page_fault);
}

int hal_inb(int port)
{
    return (int)i686_inb((uint16_t)port);
}

void hal_outb(int port, int value)
{
    i686_outb((uint16_t)port, (uint8_t)value);
}

int hal_io_wait_keyboard()
{
    return i686_io_wait_keyboard();
}

void hal_shutdown()
{
    i686_outw((uint16_t)0x604, (uint16_t)0x2000);
}

int hal_disk_init(FAT_DATA* data, DISK* disk, int test_read)
{
    i686_detect_drives();
    /*printf("[LOG]Resetting Floppy Drive.");
    if (i686_floppy_reset(FLOPPY_STANDARD_BASE))
    {
        printf("[ERROR]Floppy Drive reset failed.");
        return 1;
    }
    printf("[LOG]Floppy Drive successfully reset.");*/

    if (test_read)
    {
        printf("[LOG]Floppy Drive: proceed with test read, cylinder = 0.");
        
        void* buffer = malloc(512);
        if (i686_floppy_read_sector(disk, FLOPPY_STANDARD_BASE, 0, buffer))
        {
            printf("[ERROR]Floppy Drive test read failed.");
            free(buffer);
            return 1;
        }
        free(buffer);

        printf("[LOG]Floppy Drive test read passed.");
    }

    fat_init(data);
    printf("[LOG]FAT initialised.");

    return 0;
}

/*void hal_page_init()
{
    i686_page_init();

    uint32_t* page_table __attribute__((aligned(4096)));
    size_t block_size;
    page_table = (uint32_t*)buddy_alloc(4096, &block_size);
    i686_page_table(page_table, 0);

    i686_page_enable();
}*/