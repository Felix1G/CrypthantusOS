#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

enum PAGE_BITS
{
    PAGE_PRESENT        = 0x01,
    PAGE_READ_WRITE     = 0x02, //false = read only
    PAGE_ALL_ACCESS     = 0x04, //false = kernel mode only
    PAGE_PWT            = 0x08, //true = write-through caching, false = write back
    PAGE_PCD            = 0x10, //true = not cached, false = cached
    PAGE_ACCESSED       = 0x20, 
    PAGE_AVAILABLE      = 0x40, 
};

void page_init();
void page_table(uint32_t* table, int index, int extraflags);
void page_table_entry(uint32_t* table, int index, size_t maddr); //mapped address
void page_enable();
int __attribute__((cdecl)) _load_page_dir(uint32_t* directory);
void __attribute__((cdecl)) _enable_paging(void);