#include "hal.h"
#include "fat.h"
#include <stdlib.h>
#include <arch/i686/io.h>
#include <arch/i686/gdt.h>
#include <arch/i686/idt.h>
#include <arch/i686/irq.h>
#include <arch/i686/page.h>
#include <arch/i686/floppy.h>

void hal_init()
{
    printf("[LOG]Initialising Global Descriptor Table.");
    i686_gdt_init();
    
    printf("[LOG]Initialising Interrupt Service Routine.");
    i686_isr_init();
    i686_isr_reg_handler(0x88, _hal_isr_syscall);

    printf("[LOG]Initialising Interrupt Requests.");
    i686_irq_init();
    i686_irq_reg_handler(1, _hal_irq_keyboard);

    printf("[LOG]Loading Instruction Descriptor Table.");
    i686_idt_init();
}

int hal_inb(int port)
{
    return (int)i686_inb((uint16_t)port);
}

void hal_outb(int port, int value)
{
    i686_outb((uint16_t)port, (uint8_t)value);
}

void hal_wait_interrupt()
{
    i686_wait_interrupt();
}

int hal_wait_keyboard_interrupt()
{
    // First, reset the is_keyboard_intr state.
    // Call sti hlt.
    // When a keyboard interrupt occurs, the is_keyboard_intr will become true.
    // As only one interrupt can pass the hlt,
    // is_keyboard_intr guarantees a keyboard interrupt call.
    i686_isr_reset_keyboard_intr();
    i686_wait_keyboard_interrupt();
    return i686_isr_get_keyboard_intr();
}

void hal_disable_interrupt()
{
    i686_disable_interrupt();
}

void hal_enable_interrupt()
{
    i686_enable_interrupt();
}

void hal_shutdown()
{
    i686_outw((uint16_t)0x604, (uint16_t)0x2000);
}

int hal_disk_init(FAT_DATA* data, DISK* disk, int test_read)
{
    i686_detect_drives();
    printf("[LOG]Resetting Floppy Drive.");
    if (i686_floppy_reset(FLOPPY_STANDARD_BASE))
    {
        printf("[ERROR]Floppy Drive reset failed.");
        return 1;
    }
    printf("[LOG]Floppy Drive successfully reset.");

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

void hal_page_init()
{
    i686_page_init();
}

void hal_set_page_table(uint32_t* page_table, int index, int usermode)
{
    i686_page_table(page_table, index, (usermode ? PAGE_ALL_ACCESS : 0x00) | PAGE_READ_WRITE | PAGE_PRESENT);
}

uint32_t* hal_get_page_table(int index)
{
    return i686_get_page_table(index);
}

void hal_set_page_table_entry(uint32_t* page_table, int index, size_t mapped_addr, int usermode, int readonly)
{
    i686_page_table_entry(page_table, index, mapped_addr, 
        (usermode ? PAGE_ALL_ACCESS : 0x00) |
        (readonly ? 0x00 : PAGE_READ_WRITE) | PAGE_PRESENT);
}

uint32_t hal_physical_addr(uint32_t vir_addr)
{
    return i686_physical_addr(vir_addr);
}
