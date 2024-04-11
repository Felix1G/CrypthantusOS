#include "page.h"

static uint32_t* g_page_dir;

void i686_page_init()
{
    g_page_dir = _get_page_dir();
}

void i686_page_table(uint32_t* table, int index, int flags)
{
    g_page_dir[index] = ((uint32_t)table) | flags;
}

uint32_t* i686_get_page_table(int index)
{
    return (uint32_t*)(g_page_dir[index] & (~0xFFF));
}

int i686_page_table_find_empty(uint32_t* table)
{
    for (int i = 0;i < 1024;i++)
        if ((table[i] & 0xFFFFF000) == 0)
            return i;
    return -1;
}

void i686_page_table_entry(uint32_t* table, int index, size_t maddr, int flags)
{
    //set top 4KB address, read/write and present
    table[index] = (maddr & (~0xFFF)) | flags;
}

uint32_t i686_physical_addr(uint32_t vir_addr)
{
    uint32_t table_entry = g_page_dir[vir_addr >> 22];
    uint32_t* table_addr = (uint32_t*)(table_entry & (~0x3FF));
    uint32_t page = table_addr[(vir_addr >> 12) & 0x3FF];
    return (page & ~0x3FF) | (vir_addr & 0xFFF);
}