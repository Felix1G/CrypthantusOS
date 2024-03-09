#pragma once
#include <boot/bootlib.h>

int buddy_init(BOOT_DATA* boot_data);
void *buddy_alloc(unsigned size);
void buddy_free(void *ptr);