#include "page.h"

uint32_t g_page_dir[1024] __attribute__((aligned(4096)));

void page_init()
{
    for (int i = 0;i < 1024;i++)
    {
        g_page_dir[i] = PAGE_READ_WRITE; //kernel mode, read/write, not present
    }
}

void page_table(uint32_t* table, int index, int extraflags)
{
    g_page_dir[index] = ((uint32_t)table) | PAGE_READ_WRITE | PAGE_PRESENT | extraflags;
}

void page_table_entry(uint32_t* table, int index, size_t maddr)
{
    //set top 4KB address, read/write and present
    table[index] = (maddr & 0xFFFFF000) | PAGE_READ_WRITE | PAGE_PRESENT;
}

int page_enabled = 0;

void page_enable()
{
    if (page_enabled)
        return;
    page_enabled = 1;

    _load_page_dir(g_page_dir);
    _enable_paging();
}