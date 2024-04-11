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

void i686_page_init();
void i686_page_table(uint32_t* table, int index, int flags);
uint32_t* i686_get_page_table(int index);
void i686_page_table_entry(uint32_t* table, int index, size_t maddr, int flags); //mapped address
uint32_t* __attribute__((cdecl)) _get_page_dir();

uint32_t i686_physical_addr(uint32_t vir_addr);

//returns index of page table that is empty, no index = -1
int i686_page_table_find_empty(uint32_t* table);