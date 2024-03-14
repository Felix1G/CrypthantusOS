#pragma once
#include <boot/bootlib.h>
#include <stddef.h>

int buddy_init(BOOT_DATA* boot_data);
void *buddy_alloc(size_t size, size_t* block_size);
void buddy_free(void *ptr);